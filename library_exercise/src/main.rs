use std::process::exit;
use std::path::Path;
use std::fs::{File, OpenOptions};
use serde::{Serialize, Deserialize};
use clap::{App, Arg, SubCommand, ArgMatches, value_t_or_exit, AppSettings};


#[derive(Serialize, Deserialize)]
struct Book {
    id: u32,
    name: String,
    author: String,
    copies_total: u32,
    copies_lent: u32,
}

#[derive(Serialize, Deserialize)]
struct Library {
    books: Vec<Book>,
}


fn parse_args() -> ArgMatches<'static> {
    App::new("Library")
        .version("0.0.1")
        .author("Tomasz Necio <tomasz.necio@gmail.com>")
        .about("Library app for ZC Exercise 3")
        .setting(AppSettings::SubcommandRequiredElseHelp)
        .arg(Arg::with_name("file")
            .short("f")
            .value_name("PATH")
            .help("Path to the config file.")
            .default_value("library.json")
            .takes_value(true))

        .subcommand(SubCommand::with_name("lend")
            .about("Rent a copy of a book")
            .arg(Arg::with_name("ID")
                .required(true)
                .help("Book ID")))

        .subcommand(SubCommand::with_name("return")
            .about("Return a copy of a book")
            .arg(Arg::with_name("ID")
                .required(true)
                .help("Book ID")))

        .subcommand(SubCommand::with_name("list")
            .about("List all books in the library"))

        .get_matches()
}

fn fail_gracefully(display_str: &str) -> ! {
    eprintln!("{}", display_str);
    exit(1)
}

fn main() {
    let args = parse_args();
    let path = args.value_of("file").unwrap_or("library.json");
    let path = Path::new(path);

    match args.subcommand() {
        ("lend", Some(subcmd_args)) => lend(subcmd_args, &path),
        ("return", Some(subcmd_args)) => return_book(subcmd_args, &path),
        ("list", _) => list(&path),
        _ => {
            println!("{}", args.usage());
            exit(1);
        }
    };
}

fn read_library(path: &Path) -> Library {
    let config_file = File::open(path)
        .unwrap_or_else(|_| fail_gracefully("Cannot read file"));
    serde_json::from_reader(config_file)
        .unwrap_or_else(|_| fail_gracefully("Cannot parse file"))
}

fn write_library(path: &Path, library: &Library) {
    let config_file = OpenOptions::new()
        .create(true)
        .write(true)
        .truncate(true)
        .open(path)
        .unwrap_or_else(|_| fail_gracefully("Cannot open file for writing"));
    serde_json::to_writer_pretty(config_file, library)
        .unwrap_or_else(|_| fail_gracefully("Cannot save library to file"));
}

fn lend(args: &ArgMatches, config_file_path: &Path) {
    let book_id: u32 = value_t_or_exit!(args, "ID", u32);
    let mut library = read_library(config_file_path);
    let mut book = library.books.iter_mut().filter(|x| x.id == book_id).nth(0)
        .unwrap_or_else(|| fail_gracefully("Book ID not found"));

    if book.copies_lent == book.copies_total {
        eprintln!("All copies of {} are lent out.", book.name);
        exit(1);
    }

    book.copies_lent += 1;
    write_library(config_file_path, &library);
}

fn return_book(args: &ArgMatches, config_file_path: &Path) {
    let book_id: u32 = value_t_or_exit!(args, "ID", u32);
    let mut library = read_library(config_file_path);
    let mut book = library.books.iter_mut().filter(|x| x.id == book_id).nth(0)
        .unwrap_or_else(|| fail_gracefully("Book ID not found"));

    if book.copies_lent == 0 {
        eprintln!("No copies of {} are lent out.", book.name);
        exit(1);
    }

    book.copies_lent -= 1;
    write_library(config_file_path, &library);
}

fn list(config_file_path: &Path) {
    let library = read_library(config_file_path);
    macro_rules! table_fmt { () => ("{0: <5} | {1: <25} | {2: <25} | {3: <10} | {4: <10}") }
    println!(table_fmt!(), "ID", "Name", "Author", "Lent out", "Available");
    println!("{}", (0..80).map(|_| "-").collect::<String>());
    library.books.iter().for_each(|book| {
        println!(table_fmt!(), book.id, book.name, book.author, book.copies_lent,
                 book.copies_total - book.copies_lent);
    });
}
