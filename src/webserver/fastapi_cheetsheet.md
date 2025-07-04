
FastAPI Cheat Sheet

1. Installation:
   - Install FastAPI using pip:
     ```
     $ pip install fastapi
     ```

2. Importing FastAPI:
   - Import the FastAPI module in your Python script:
     ```python
     from fastapi import FastAPI
     ```

3. Creating an App:
   - Create an instance of the FastAPI class to represent your application:
     ```python
     app = FastAPI()
     ```

4. Creating Routes:
   - Define routes using the `@app.route()` decorator and specify the HTTP methods:
     ```python
     @app.route("/path", methods=["GET"])
     async def endpoint_name():
         # Endpoint logic goes here
         return {"message": "Hello, FastAPI!"}
     ```

5. Request Parameters:
   - Access request parameters using function parameters:
     ```python
     @app.route("/items/{item_id}")
     async def get_item(item_id: int):
         # Access item_id parameter
         return {"item_id": item_id}
     ```

6. Query Parameters:
   - Define query parameters using function parameters with default values:
     ```python
     @app.route("/items")
     async def get_items(skip: int = 0, limit: int = 10):
         # Access skip and limit query parameters
         return {"skip": skip, "limit": limit}
     ```

7. Request Body:
   - Define request body models using Pydantic models:
     ```python
     from pydantic import BaseModel

     class Item(BaseModel):
         name: str
         price: float

     @app.route("/items", methods=["POST"])
     async def create_item(item: Item):
         # Access item request body
         return {"item": item}
     ```

8. Response Models:
   - Define response models using Pydantic models:
     ```python
     class Item(BaseModel):
         name: str
         price: float

     @app.route("/items/{item_id}")
     async def get_item(item_id: int):
         # Return item response with model
         return {"item_id": item_id, "name": "Item Name", "price": 9.99}
     ```

9. Path Operations:
   - Use different HTTP methods for the same path by defining separate route functions:
     ```python
     @app.route("/items/{item_id}", methods=["GET"])
     async def get_item(item_id: int):
         # Get item logic

     @app.route("/items/{item_id}", methods=["PUT"])
     async def update_item(item_id: int, item: Item):
         # Update item logic
     ```

10. Middleware:
    - Use middleware functions to intercept and modify requests and responses:
      ```python
      @app.middleware("http")
      async def middleware(request, call_next):
          # Modify request before passing to route
          response = await call_next(request)
          # Modify response before returning
          return response
      ```

11. Exception Handling:
    - Use exception handlers to handle specific exceptions raised within routes:
      ```python
      @app.exception_handler(ExceptionType)
      async def exception_handler(request, exc):
          # Handle exception and return custom response
          return JSONResponse(status_code=400, content={"message": "Error"})
      ```

12. Running the App:
    - Start the FastAPI application using the `uvicorn` command:
      ```
      $ uvicorn main:app --reload
      ```
      Replace `main` with the name of your Python script and `app` with the name of your FastAPI instance.

This cheat sheet covers the basics of FastAPI to help you get started with building web APIs using the framework. For more detailed information, refer to the official FastAPI documentation.
