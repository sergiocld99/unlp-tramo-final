fn main() {
    println!("Hello, world!");

    // vamos a realizar un producto cartesiano
    let arr1 = [1, 2, 3, 4];
    let arr2 = [5, 6, 7, 8];
    let mut results: Vec<i32> = Vec::new();

    for i in 0..arr1.len() {
        for j in 0..arr2.len() {
            let aux = arr1[i] * arr2[j];
            results.push(aux)
        }
    }

    for k in results.iter() {
        println!("{}", k)
    }
}
