use std::path::Path;
use image::{ImageError, ExtendedColorType};

pub fn mix(mut h: u32) -> u32 {
	h ^= h >> 16;
	h = h.wrapping_mul(2246822507);
	h ^= h >> 13;
	h = h.wrapping_mul(3266489909);
	h ^= h >> 16;
	
	return h;
}

// Generate Identicon;
pub fn gen_identicon(seed: u64, border_offset: usize, pixel_width: usize, grid_width: usize, color: [u8; 3], background: [u8; 3]) -> Vec<u8> {
	let mut buf = vec![0u8; grid_width * grid_width * 3];
	let mut z = seed as u32;

	// Generate Half;
	let mut y = border_offset;

	while y + pixel_width <= grid_width - border_offset {
		let mut x = border_offset;

		while x + pixel_width <= grid_width / 2 {
			z = mix(z);
			let raw = (z >> 2) & 6;
			let pixel_color = if raw > 5 { color } else { background };

			// Draw Pixel Block;
			let mut py = 0;

			while py < pixel_width {
				let mut px = 0;

				while px < pixel_width {
					let idx = ((y + py) * grid_width + (x + px)) * 3;
					buf[idx..idx + 3].copy_from_slice(&pixel_color);

					px += 1;
				}

				py += 1;
			}

			x += pixel_width;
		}

		y += pixel_width;
	}

	// Mirror (Left To Right);
	let (half_width, mut n) = (grid_width / 2, 0);

	while n < grid_width {
		let mut h = 0;

		while h < half_width {
			let (l, c) = ((n * grid_width + h) * 3, grid_width - 1 - h); let r = (n * grid_width + c) * 3;

			let pixel = [buf[l], buf[l + 1], buf[l + 2]];
			buf[r..r + 3].copy_from_slice(&pixel);
			
			h += 1;
		}

		n += 1;
	}

	return buf;
}

// Result To PNG;
pub fn icon_buff_to_png<P: AsRef<Path>>(icon: &[u8], icon_width: u32, path: P) -> Result<(), ImageError> {
	image::save_buffer(path, icon, icon_width, icon_width, ExtendedColorType::Rgb8)
}

// Result To JPEG;
pub fn icon_buff_to_jpg<P: AsRef<Path>>(icon: &[u8], icon_width: u32, path: P) -> Result<(), ImageError> {
	image::save_buffer(path, icon, icon_width, icon_width, ExtendedColorType::Rgb8)
}