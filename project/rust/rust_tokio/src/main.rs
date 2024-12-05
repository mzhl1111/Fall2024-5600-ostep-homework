use tokio;
use warp::Filter;

#[tokio::main(flavor = "multi_thread", worker_threads = 100)]
async fn main() {
    // Define a simple GET endpoint at "hello" that returns a string "Hello, World!"
    let hello = warp::path("hello").map(|| warp::reply::html("Hello, World!"));

    // Start the warp server on port 8080
    warp::serve(hello).run(([127, 0, 0, 1], 8080)).await;
}
