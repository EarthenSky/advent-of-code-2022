
// to run (on wsl):
// rustc day17.rs -C debuginfo=2 -C opt-level=3; time ./day17 > out17

const ROCK_SHAPES: [(usize, &str); 5] = [
    (4, "####"), (3, ".#.###.#."), (3, "..#..####"), (1, "####"), (2, "####")
];

#[derive(Eq, PartialEq)]
enum Space {
    Empty,
    Filled,
    Wall
}

fn main() {
    let mut part1: usize = 0;
    let mut part2: usize = 0;

    let input = std::fs::read_to_string("input17").expect("failed to read input");
    let input = input.replace("\n", "");

    // ---------------------------
    // content:

    //part1 = run_simulation(&input, 2022, false);
    part2 = run_simulation(&input, 10000, false); //1_000_000_000_000);

    println!("part1: {}", part1);
    println!("part2: {}", part2);
}

fn run_simulation(input: &str, num_rocks: usize, show_tower: bool) -> usize {
    let tower_width = 9;
    let mut top_filled = 1;
    let mut tower: Vec<Space> = Vec::new();
    for i in 0..(4*tower_width*(num_rocks+1)) {
        let item = if i % tower_width == 0 {
            Space::Wall
        } else if i % tower_width == tower_width-1 {
            Space::Wall
        } else if i < tower_width {
            Space::Wall
        } else {
            Space::Empty
        };
        tower.push(item);
    }

    println!("filled vector");

    //let mut column_tops: [usize; 7] = [1,1, 1,1,1, 1,1]; 
    // (slice of important string, num blocks until this point, top_filled at this point)
    let mut fingerprints: Vec<(&[Space], usize, usize)> = Vec::new();

    // idea: if our topology ever repeats, we can take advantage of that -> we need to pray that this is the case
    // I did a test, and it is the case! data will repeat after ~5k height using real input.

    // approach: 
    // - once we find a repetition, count the number of blocks that needed to be placed between them, as well as how much higher it gets
    // - use how many blocks it took to get to the old repeated point, as well as the number of blocks needed to make a repetition in order
    //   to compute how many more blocks we need to simulate to get to the max number. Then, simulate only that number. Woo, done! O(time_to_repeat)
    
    let mut timestep: u64 = 0;
    for rock in 0..num_rocks {
        // spawn rock
        let rock_kind = rock % ROCK_SHAPES.len();
        let rock_width = ROCK_SHAPES[rock_kind].0;
        let rock_pattern = ROCK_SHAPES[rock_kind].1;
        let rock_height = rock_pattern.len() / rock_width;

        let mut rock_bottom = top_filled + 3; // TODO: this can be made smaller, if we update timestep accordingly
        let mut rock_left = 3; // two spaces from the wall

        // iterate rock
        let mut placed = false;
        while !placed {
            // pushed by jet
            let action = input.chars().nth((timestep % (input.len() as u64)) as usize).unwrap(); // TODO: do I need usize?
            if action == '<' {
                if rock_left <= 1 {
                    // do nothing
                } else if intersects(rock_kind, rock_left-1, rock_bottom, &tower, tower_width) == false {
                    rock_left -= 1;
                }
            } else {
                if rock_left+rock_width >= tower_width-1 {
                    // do nothing
                } else if intersects(rock_kind, rock_left+1, rock_bottom, &tower, tower_width) == false {
                    rock_left += 1;
                }
            }

            // TODO: optimize for taking nsteps at once (raycasting) -> keep note of highest in each column
            
            // try move down
            if intersects(rock_kind, rock_left, rock_bottom-1, &tower, tower_width) {
                // add to tower
                for i in 0..rock_pattern.len() {
                    let ch = rock_pattern.chars().nth(i);
                    let x = i % rock_width; 
                    let y = (rock_height-1) - ((i / rock_width) as usize);
                    if ch == Some('#') {
                        tower[(rock_bottom+y) * tower_width + (rock_left+x)] = Space::Filled;
                    }
                }

                placed = true;
            } else {
                rock_bottom -= 1;
            }

            timestep += 1;
        }

        // check for new tallest (TODO: just compute this when overlaying...)
        for i in (top_filled*tower_width..(top_filled+5)*tower_width).rev() {
            if tower[i] == Space::Filled {
                top_filled = (i / tower_width) as usize + 1;
                break;
            }
        }

        // TODO: do a floodfill to find the deepest location we care about & create a fingerprint

        //println!("top: {}", top_filled);
    }
    
    if show_tower {
        let mut index = 0;
        for el in tower {
            if el == Space::Wall {
                print!("|");
            } else if el == Space::Empty {
                print!(".");
            } else {
                print!("#");
            }
            if index % tower_width == tower_width-1 {
                print!("\n");
            }
            index += 1;
        }
    }

    return top_filled-1;
}

fn intersects(shape_kind: usize, shape_left: usize, shape_bottom: usize, tower: &Vec<Space>, tower_width: usize) -> bool {
    let rock_width = ROCK_SHAPES[shape_kind].0;
    let rock_pattern = ROCK_SHAPES[shape_kind].1;
    let rock_height = rock_pattern.len() / rock_width;
    
    for i in 0..rock_pattern.len() {
        let ch = rock_pattern.chars().nth(i);
        let x = i % rock_width; 
        let y = (rock_height-1) - ((i / rock_width) as usize);
        if ch == Some('#') && tower[(shape_bottom+y) * tower_width + (shape_left+x)] != Space::Empty {
            //println!("{}, {}, {}", x, y, i);
            return true;
        }
    }

    false
}