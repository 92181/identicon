#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define GRID_WIDTH 512
#define PIXEL_SIZE (GRID_WIDTH / 16)

unsigned int mix(unsigned int h) 
{
	h ^= h >> 16;
	h *= 2246822507U;
	h ^= h >> 13;
	h *= 3266489909U;
	h ^= h >> 16;

	return h;
}

// Generate Identicon;
char *identi_as_buffer(unsigned long seed, int border_offset, int pixel_width, int grid_width, char *color, char *background)
{
	unsigned int z = seed; int s = grid_width * grid_width * 3, pixel_height = grid_width * pixel_width * 3; 
	
	grid_width *= 3; pixel_width *= 3;

	int border_offset_x = border_offset * 3, border_offset_y = border_offset * grid_width;

  // Generate Half;
  char *m = calloc(1, s), *n = m + border_offset_y;

	if(m == 0)
	{
		return 0;
	}

  while(n < m + s - border_offset_y)
	{
		// Every Half;
		char *h = n + border_offset_x;

		while(h < n + grid_width / 2)
		{
			char raw, r = 0, g = 0, b = 0;

			z = mix(z); raw = (z >> 2) & 6;

			if(raw > 5)
			{
				r = color[0]; g = color[1]; b = color[2];
			}
			else
			{
				r = background[0]; g = background[1]; b = background[2];
			}
			
			// Draw Pixel Block;
			char *x = h, *y;

			while(x < h + pixel_height)
			{
				y = x;

				while(y < x + pixel_width)
				{
					*y = r; *(y + 1) = g; *(y + 2) = b; y += 3;
				}

				x += grid_width;
			}

			h += pixel_width;
		}

		n += pixel_height;
	}

	// Mirror (Left To Right);
	char *j = m + grid_width - 3; n = m;

	while(n < m + s - grid_width / 2)
	{
		char *h = j, *x = n;

		while(x < n + grid_width / 2)
		{
			*h = *x; *(h + 1) = *(x + 1); *(h + 2) = *(x + 2); h -= 3; x += 3;
		}
	
		j += grid_width; n += grid_width;
	}

	return m;
}

// Result To BMP;
#pragma pack(push, 1)
typedef struct {
	unsigned short type;
	unsigned int size;
	unsigned short reserved1, reserved2;
	unsigned int offset;
} BMPFileHeader;

typedef struct {
	unsigned int info_size;
	int width, height;
	unsigned short planes, bit_count;
	unsigned compression;
	unsigned size_image;
	int x_pixels_meter, y_pixels_meter;
	unsigned int used_colors;
	unsigned int imp_colors;
} BMPInfoHeader;
#pragma pack(pop)

void icon_buff_to_bmp(char *icon, int icon_width, char *path)
{
	int f = open(path, O_CREAT | O_RDWR, 0644);

	if(f > 0)
	{
		int icon_size = icon_width * icon_width * 3;

		BMPFileHeader g = {
			.type = 0x4D42,
			.size = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + icon_size,
			.offset = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader)
		};

		BMPInfoHeader h = {
			.info_size = sizeof(BMPInfoHeader),
			.width = icon_width,
			.height = -icon_width,
			.planes = 1,
			.bit_count = 24,
			.compression = 0,
			.size_image = icon_size
		};

		write(f, &g, sizeof(BMPFileHeader));
  	write(f, &h, sizeof(BMPInfoHeader));

		// Switch RGB To BRG (BMP Specific);
		char *i = icon;

		while(i < icon + icon_size)
		{
			char c[3] = {*(i + 2), *(i + 1), *i}; write(f, c, 3);

			i += 3;
		}

		close(f);
	}
}

// Result To PNG;
void icon_buff_to_png(char *icon, int icon_width, char *path)
{
	stbi_write_png(path, icon_width, icon_width, 3, icon, icon_width * 3);
}

// Result To JPG;
void icon_buff_to_jpg(char *icon, int icon_width, char *path)
{
	stbi_write_jpg(path, icon_width, icon_width, 3, icon, icon_width * 3);
}

int main()
{
	struct timespec s, e; clock_gettime(1, &s);
	
	// Generate Icon As Image Buffer;
	char color[4] = {0}, background[4] = {0, 0, 0}; color[0] = 0; color[1] = 0; color[2] = 255;

	char *icon = identi_as_buffer(s.tv_nsec, PIXEL_SIZE, PIXEL_SIZE, GRID_WIDTH, color, background);

	if(icon != 0)
	{
		// Print Result;
		icon_buff_to_png(icon, GRID_WIDTH, "./output.png");
		icon_buff_to_bmp(icon, GRID_WIDTH, "./output.bmp");

		// Free;
		free(icon);
	}
	
	return 0;
}