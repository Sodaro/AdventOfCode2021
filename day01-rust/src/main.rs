use std::fs::File;
use std::io::{self, BufRead};
use std::path::Path;
use std::fs;

fn get_sum_range(values: &[i32]) -> i32
{
    let mut sum = 0;
    for value in values
    {
        sum += *value;
    }
    return sum;
}

fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<File>>>
where P: AsRef<Path>, {
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}

fn solve_part1(input: &str, output: &str)
{
    if let Ok(lines) = read_lines(input) {
        let mut last_depth = 0;
        let mut increase_count = 0;
        for line in lines
        {
            if let Ok(valid_line) = line{
                let depth_value = valid_line.parse().unwrap();
                if last_depth != 0 && depth_value > last_depth
                {
                    increase_count+=1;
                }
                last_depth = depth_value;
            }
        }
        write_to_file(increase_count, output);
    }
}

fn solve_part2(input: &str , output: &str)
{
    if let Ok(lines) = read_lines(input) {
        let mut depths: Vec<i32> = Vec::new();
        let mut increase_count = 0;
        for line in lines
        {
            if let Ok(line_value) = line{
                depths.push(line_value.parse().unwrap());
                if depths.len() == 4
                {
                    let a = get_sum_range(&depths[0..3]);
                    let b = get_sum_range(&depths[1..4]);
                    if b > a {
                        increase_count += 1;
                    }
                    depths.remove(0);
                }
            }
        }
        write_to_file(increase_count, output);
    }
}
fn write_to_file(number: i32, path: &str){
    fs::write(path, number.to_string()).expect("Unable to write file");
}

fn main()
{
    solve_part1("input.txt", "output1.txt");
    solve_part2("input.txt", "output2.txt");
}
