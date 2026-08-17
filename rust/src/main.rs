use std::time::{Instant};

const GRID_WIDTH: usize = 512;
const PIXEL_SIZE: usize = GRID_WIDTH / 16;

fn main() {
	let seed = Instant::now().elapsed().as_nanos() as u64;

	let (color, background) = ([0, 0, 255], [0, 0, 0]);

	// Generate Icon
	let icon = cr_identicon::gen_identicon(seed, PIXEL_SIZE, PIXEL_SIZE, GRID_WIDTH, color, background);

	// Save Output Files
	cr_identicon::icon_buff_to_png(&icon, GRID_WIDTH as u32, "./output.png").expect("Failed to save PNG");
	cr_identicon::icon_buff_to_jpg(&icon, GRID_WIDTH as u32, "./output.jpg").expect("Failed to save JPG");
}