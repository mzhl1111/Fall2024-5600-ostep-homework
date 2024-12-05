package main

import (
    "github.com/gin-gonic/gin"
)

func main() {
    r := gin.Default()

    // Define a simple GET endpoint
    r.GET("/", func(c *gin.Context) {
        c.JSON(200, gin.H{
            "message": "Hello, World!",
        })
    })

    // Run the server on port 8080
    r.Run(":8080")
}
