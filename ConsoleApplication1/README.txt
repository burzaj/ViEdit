       
"atk_dither" : || atkinson dithering for entaire pixels
"treshhold"  : |+/- value| +: every pixel with graynes < value is set to black, -: every pixel with graynes > value is set to black
"save"       : |int/float| saves a snapshot every int frames/ every float * frame_count
"palette"    : |path/name/r,g,b,r,g,b...| applyes a pallete path : from path. name : from Palettes folder. r,g,b... : manual
"reset"      : || resets the working frame buffer
"norm_bright" : || normalize colors to maximum brightness
"atk_col_dither" : || atkinson dithering for color channels
"gauss_blur" : |float| applyes gaussuian blur of float strength 
"abort" : |int/float| stops processing and performs a hard finish after int frames / float * frame_count
"skip" : |int/float/end, int2| skips int2 effects for int / float*frame_count / all frames if arg1 negative skips after that amount of frames
"start" : |int/float| does not save first int/float*frame_count frames
"rect" : |x1,x2,y1,y2,line,r,g,b| draws a rectangle 
"text" : |font,x,y,text,s=float,v_center,h_center,r,g,b| draws text at x,y
"log" : |real,imag| experimental complex log of an image
"pow" : |real,imag| experimental complex power of an image


"scale" : |width,height or float| scales the image
"vertical" : || quick fix for videos recorded verticaly