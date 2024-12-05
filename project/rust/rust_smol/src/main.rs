use async_std::task;
use async_std::net::TcpListener;
use async_h1::server;
use http_types::{Request, Response, StatusCode};
use smol::spawn;

// The main function to start the server
fn main() {
    // Using async_std's task to execute our async code.
    task::block_on(async {
        // Bind to the TCP address
        let listener = TcpListener::bind(([127, 0, 0, 1], 8080)).await.expect("Could not bind to address");
        println!("Server listening on http://127.0.0.1:8080");

        // Loop to accept incoming connections
        loop {
            let (stream, _) = listener.accept().await.expect("Failed to accept connection");
            spawn(handle_request(stream)).detach();
        }
    });
}

// The function to handle incoming requests
async fn handle_request<T: async_std::io::Read + async_std::io::Write + Unpin + Send + Sync + 'static>(stream: T) {
    if let Err(e) = server::accept(stream, request_handler).await {
        eprintln!("Error handling request: {}", e);
    }
}

// A basic function that will create a response based on a request.
async fn request_handler(_req: Request) -> http_types::Result<Response> {
    let mut response = Response::new(StatusCode::Ok);
    response.set_body("Hello from smol!");
    Ok(response)
}
