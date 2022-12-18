/*
rustc print_example.rs && ./print_example
company name is:TutorialsPoint
company rating on 5 is:4.5
company is growing :true
company icon is:♥
 * */

fn main() {
   let company_string = "TutorialsPoint";  // string type
   let rating_float = 4.5;                 // float type
   let is_growing_boolean = true;          // boolean type
   let icon_char = '♥';                    //unicode character type

   println!("company name is:{}",company_string);
   println!("company rating on 5 is:{}",rating_float);
   println!("company is growing :{}",is_growing_boolean);
   println!("company icon is:{}",icon_char);
}

