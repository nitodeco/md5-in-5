use std::env;
mod md5;

fn main() {
    let args: Vec<String> = env::args().collect();
    
    if args.len() != 2 {
        eprintln!("Usage: {} <string_to_hash>", args[0]);
        std::process::exit(1);
    }

    let input = &args[1];
    let hash = md5::md5(input);
    println!("{}", hash);
}
