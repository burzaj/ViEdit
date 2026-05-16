#include "Process.h"

#include <filesystem>

#include "Macros.h"
#include "FFmpegWrappers.h"
#include "Effect.h"
#include "process.hpp"
#include <mutex>
#include <condition_variable>

namespace fs = std::filesystem;

static void write_progress(const size_t& frame_count, 
    const size_t& frames, 
    const int& curr_proc, 
    const int& max_proc, 
    const int& curr_file, 
    const int& max_file)
{
    auto percent_progress = std::to_string((int)((float)(frame_count) / (float)(frames) * 100)) + "%";
    auto frame_count_str = std::to_string(frame_count);
    auto frames_str = std::to_string(frames);

    int percent_size_diff = 4 - percent_progress.size();

    for (int i = 0; i < percent_size_diff; i++)
        percent_progress = " " + percent_progress;

    int frame_size_diff = frames_str.size() - frame_count_str.size();

    for (int i = 0; i < frame_size_diff; i++)
        frame_count_str = " " + frame_count_str;

    std::cout << "\r" << "\033[1;32m" << "[" << curr_file << "/" << max_proc << "][" << curr_proc << "/" << max_proc <<
        "][" << percent_progress << "][" << frame_count_str << "/" << frames_str << "]" << "\033[0m" << std::flush;
}

std::string Process::start(const bool& terminal, const int& curr_proc, const int& max_proc, const int& curr_file, const int& max_file) {
    size_t frame_size = width * height * 3;
    std::vector<char> buffer;
    std::mutex buffer_mutex;
    std::condition_variable cv;
    bool input_finished = false;

    int exit_code;

    // 1. Start the INPUT process (FFmpeg -> App)
    TinyProcessLib::Process ffmpeg_in(input_cmd, L"",
        [&](const char* bytes, size_t n) {
            std::lock_guard<std::mutex> lock(buffer_mutex);
            buffer.insert(buffer.end(), bytes, bytes + n);
            cv.notify_one(); // Tell the loop data is ready
        }, nullptr, false);

    std::string ffmpeg_errors;

    // 2. Start the OUTPUT process (App -> FFmpeg)
    // The 'true' at the end enables writing to stdin
    TinyProcessLib::Process ffmpeg_out(output_cmd, L"", nullptr, 
        [&](const char* bytes, size_t n) {
            ffmpeg_errors += std::string(bytes, n);
        }, 
        true);

    Frame original_frame;
    Frame target_frame;

    original_frame.construct(width, height);
    target_frame.construct(width, height);

    while (true) {
        std::unique_lock<std::mutex> lock(buffer_mutex);

        // Wait until we have a full frame or the process ends
        cv.wait(lock, [&] {
            return buffer.size() >= frame_size || ffmpeg_in.try_get_exit_status(exit_code);
            });

        if (buffer.size() < frame_size) break;

        // Pull one frame out of the buffer
        std::copy(buffer.begin(), buffer.begin() + frame_size, (char*)original_frame.data());
        buffer.erase(buffer.begin(), buffer.begin() + frame_size);
        lock.unlock();

        target_frame = original_frame;
        for (auto effect : effects)
        {
            effect->apply(target_frame, original_frame, width, height, *this);

        }

        frame_count++;

        if (terminal)
            write_progress(frame_count, frames, curr_proc, max_proc, curr_file, max_file);

        ffmpeg_out.write((const char*)target_frame.data(), frame_size);
    }
    if (terminal)
        std::cout << "\n";
    ffmpeg_out.close_stdin();
    int exit_status = ffmpeg_out.get_exit_status();

    if (exit_status != 0) {
        return "[FFmpeg Error] (Exit code" + std::to_string(exit_status) + "): " + ffmpeg_errors;
    }

    return "Process finished successfully";
}

fs::path Process::peek_output()
{
    return (paths::output_dir / (output_name + extension.wstring()));
}

Process::Process(fs::path& filepath, std::vector<Effect*>& _effects, std::vector<GlobalEffect*>& _global_effects, const size_t& _num)
{
    effects = _effects;

    vertical = false;

    frames = 0;

    get_dimensions(filepath, width, height, frames, vertical);

    frames += 1;

    extension = filepath.extension().string();

    for (auto global_effect : _global_effects)
        global_effect->apply(*this);

	original_frame.construct(width, height);
	target_frame.construct(width, height);

    bool is_video = false;

    if (extension == ".mp4")
        is_video = true;

    input_cmd = L"ffmpeg -hide_banner -i \"" + filepath.wstring() + L"\" -vf \"scale=" + std::to_wstring(width) + L":" + std::to_wstring(height) + L"\" -f rawvideo -pix_fmt rgb24 -";

    output_name = L"output_" + std::to_wstring(_num);

    std::wstring str_height = std::to_wstring(height);
    std::wstring str_width = std::to_wstring(width);

    if (is_video) {
        output_cmd = L"ffmpeg -hide_banner -loglevel error -y -f rawvideo -pix_fmt rgb24 -s " +
            std::to_wstring(width) + L"x" + std::to_wstring(height) +
            L" -r " + std::to_wstring(30) + L" -i - " + // Define source_fps!
            L"-i \"" + filepath.wstring() + L"\" " +
            L"-map 0:v:0 -map 1:a:0? -c:a aac " +
            L"-c:v libx264 -pix_fmt yuv420p -c:a aac -shortest " +
            L"\"" + (paths::output_dir / output_name).wstring() + L".mp4" + L"\"";
    }
    else {
        output_cmd = L"ffmpeg -hide_banner -loglevel quiet -y -f rawvideo -pix_fmt rgb24 -s " +
            str_width + L"x" + str_height +
            L" -i - \"" + (paths::output_dir / output_name).wstring() + extension.wstring() + L"\" -c:a aac";
    }
}
