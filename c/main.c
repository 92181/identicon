#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>

#define GRID_WIDTH 512
#define PIXEL_SIZE (GRID_WIDTH / 8)

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
char *identi_as_buffer(unsigned long seed, int border_offset, int pixel_size, int grid_width, char *color, char *background)
{
	unsigned int hash = seed; int s = grid_width * grid_width * 3, pixel_height = grid_width * pixel_size * 3; 
	
	grid_width *= 3; pixel_size *= 3; 

	char *m = calloc(1, s), *n = m;

	if(m == 0)
	{
		return 0;
	}

	// Generate Half;
	while(n < m + s - grid_width / 2)
	{
		// Every Half;
		char *h = n;

		while(h < n + grid_width / 2)
		{
			char raw, r = 0, g = 0, b = 0;

			hash = mix(hash); raw = (hash >> 2) & 3; 
					
			if(raw > 2)
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

				while(y < x + pixel_size)
				{
					*y = r; *(y + 1) = g; *(y + 2) = b; y += 3;
				}

				x += grid_width;
			}

			h += pixel_size;
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
	unsigned int compression, size_image;
	int x_pixels_meter, y_pixels_meter;
	unsigned int used_colors;
	unsigned int imp_colors;
} BMPInfoHeader;
#pragma pack(pop)

void buffer_to_bmp(char *icon, int icon_width)
{
	int f = open("./output.bmp", O_CREAT | O_RDWR, 0644);

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

		write(f, icon, icon_size);

		close(f);
	}
}

// Result To SVG;
char *identi_as_svg(unsigned long seed, int pixel_size, int grid_width)
{
	/*char *m = icon;

	while(m < icon + size)
	{
		while((m - icon) % GRID_WIDTH == 0) 
		{
			printf("%d ",*m); m += PIXEL_SIZE;
		};

		m += PIXEL_SIZE; printf("\n");
	}*/
}

int main()
{
	struct timespec s, e; clock_gettime(1, &s);
	
	int icon_size = GRID_WIDTH * GRID_WIDTH;
	
	// Generate Icon As Image Buffer;
	char color[4] = {0}, background[4] = {0, 0, 0}; color[0] = 255; color[1] = 0; color[2] = 0;

	char *icon = identi_as_buffer(s.tv_nsec, PIXEL_SIZE * 2, PIXEL_SIZE, GRID_WIDTH, color, background);

	if(icon != 0)
	{
		// Print Result;
		//char *icon = identi_as_svg(s.tv_nsec, PIXEL_SIZE, GRID_WIDTH);

		buffer_to_bmp(icon, GRID_WIDTH);

		// Free;
		free(icon);
	}
	
	return 0;
}