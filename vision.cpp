#include "CImg.h"
#include <stdio.h>
//#include <intrin.h>
//TODO: Draw a box around the thing we want
//TODO: Add hotkeys to cycle pictures, etc

using namespace cimg_library;
#ifndef image_path
#define image_path "Beacon Pictures/"
#endif

static inline uint32_t rdtscp( uint32_t & aux )
{    uint32_t rax,rdx;
   asm volatile ( "rdtscp\n" : "=a" (rax), "=d" (rdx), "=c" (aux) : : );
   return rax;
}

int main(int argc, char const **argv) {
	cimg_usage("Test the vision algorithm on pictures of the beacon");

	const char* file_i = cimg_option("-i", image_path "004.jpg","Input image");

	CImg<unsigned char> image = CImg<>(file_i).resize_halfXY();

	CImgDisplay main_disp(image,"Beacon",0);
	CImgDisplay draw_disp(300, 100, "Menu", 0);
	//Color Stuff
	unsigned int hatch = 0xF0F0F0F0;
	const unsigned char
	red[]   = { 255,0,0 },
	green[] = { 0,255,0 },
	blue [] = { 0,0,255 },
	black[] = { 0,0,0 };

	CImg<unsigned char> highlight(main_disp.width(), main_disp.height(), 1,3,0);
	#define reqPixelz (image.width()*image.height()/32) //TODO: Make this smarter
	int reqPixels = reqPixelz;
	int timing[20];
	uint32_t blah;
/*Timing Stuff*/
//for(int k = 0; k < 10; k++)
//{
	//timing[k] = rdtscp(blah);

	int red_pixel_count[2], blue_pixel_count[2];
	red_pixel_count[0] = 0;
	blue_pixel_count[0] = 0;
	int red_pos[2] = {0, 0};
 	int blue_pos[2] = {0, 0};
	for(int i = 0; i < image.width()-5; i+=5)
	{
		red_pixel_count[1] = 0;//TODO: make this an array for x and y
		blue_pixel_count[1] = 0;
		for(int j = 0; j < image.height()-5; j+=5)
		{
			if(image(i, j, 0) > 200){//If at some point there is a pixel that is sufficiently red and not blue
				red_pixel_count[1]++;
				red_pixel_count[0]++;
				highlight.draw_circle(i,j,5,red,1.0f).display(main_disp);
				if(red_pixel_count[0]*red_pixel_count[1] > reqPixels)//If there are enough pixels in this row
				{
					//printf("Red Flag has reached high enough value @ %i, %i, %i, %i\n", i, j, red_pixel_count[0], red_pixel_count[1]);
					red_pixel_count[0] = 0;
					red_pixel_count[1] = 0;
					red_pos[0] = i;
					red_pos[1] = j;
				}
			}
			else if(image(i, j, 2) > 200)
			{
				blue_pixel_count[1]++;
				blue_pixel_count[0]++;
				highlight.draw_circle(i,j,5,blue,1.0f).display(main_disp);
				if(blue_pixel_count[0]*blue_pixel_count[1] > reqPixels)//If there are enough pixels in this row TODO: add y dim to this for more accuracy
				{
					//printf("Blue Flag has reached high enough value @ %i, %i, %i, %i\n", i, j, blue_pixel_count[0], blue_pixel_count[1]);
					blue_pixel_count[0] = 0;
					blue_pixel_count[1] = 0;
					blue_pos[0] = i;
					blue_pos[1] = j;
				}
			}
			else
			{
				const unsigned char colorz[] = {image(i,j,0), image(i,j,1), image(i,j,2)};
				highlight.draw_point(i, j, colorz,1.0f);//There has to be a better way than this
			}
		}

	}
	//TODO: Add beacon not found case
	if(blue_pos[0] > red_pos[0])
	{
		printf("Blue is on the right\n");
	}
	else if(red_pos[0] > blue_pos[0])
	{
		printf("Red is on the right\n");
	}
/*Timing Stuff*/
// 	timing[k+10] = rdtscp(blah);
// }
// 	float average = 0;
// 	for(int i = 0; i < 10; i++)
// 	{
// 	 	average += (timing[i+10]-timing[i])/10;
// 	}
// 	printf("Average is: %.2f\n", average);
	while (!main_disp.is_closed() && !draw_disp.is_closed() &&
	!main_disp.is_keyESC() && !draw_disp.is_keyESC() && !main_disp.is_keyQ() && !draw_disp.is_keyQ()) {
		if (main_disp.is_resized()) main_disp.resize().display(image);
		draw_disp.resize();
		if (main_disp.mouse_x()>=0 && main_disp.mouse_y()>=0)
		{
			const int
			xm = main_disp.mouse_x(),                     // X-coordinate of the mouse pointer over the image
			ym = main_disp.mouse_y(),                     // Y-coordinate of the mouse pointer over the image
			x = xm*image.width()/main_disp.width(),     // Corresponding X-coordinate of the pointed pixel in the image
			y = ym*image.height()/main_disp.height();   // Corresponding Y-coordinate of the pointex pixel in the image
			const unsigned int
			val_red   = image(x,y,0),
			val_green = image(x,y,1),
			val_blue  = image(x,y,2);

			CImg<unsigned char>(draw_disp.width(),draw_disp.height(),1,3,255).
			draw_text(30,5,"Pixel (%d,%d)={%d %d %d}",black,0,1,16,
			main_disp.mouse_x(),main_disp.mouse_y(),val_red,val_green,val_blue).
			display(draw_disp);
		}
		else
		{
			CImg<unsigned char>(draw_disp.width(),draw_disp.height()).fill(255).
			draw_text(draw_disp.width()/2 - 130,draw_disp.height()/2 - 5,"Mouse pointer is outside the image",
			black,0,1,16).display(draw_disp);
		}
		cimg::wait(20);
	}
	return 0;
}
