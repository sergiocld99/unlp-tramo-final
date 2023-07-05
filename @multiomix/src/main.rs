use std::thread;


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
        print!("{}, ", k)
    }

    println!();

    // ahora de forma paralela
    // idea: distribuir trabajo por filas (elementos de arr1)
    let t0 = thread::spawn(move || {
        // esto es una función anonima
        local_cross_product(arr1, arr2, 0)
    });

    let t1 = thread::spawn(move || {
        // esto es una función anonima
        local_cross_product(arr1, arr2, 1)
    });

    t0.join().unwrap();
    t1.join().unwrap();
}

fn local_cross_product(arr1: [i32; 4], arr2: [i32; 4], id: u32){
    let mut local_results: Vec<i32> = Vec::new();
        let ini = if id == 0 {0} else {arr1.len() / 2};
        let fin = if id == 0 {arr1.len() / 2} else {arr1.len()};

        for i1 in ini..fin {
            for j1 in 0..arr2.len() {
                let aux = arr1[i1] * arr2[j1];
                local_results.push(aux);
            }
        }

        for k1 in local_results.iter() {
            print!("{}, ", k1);
        }
}