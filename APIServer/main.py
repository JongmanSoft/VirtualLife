# api_server.py
from fastapi import FastAPI, Request
from pydantic import BaseModel

app = FastAPI()

class DataRequest(BaseModel):
    id: int
    message: str

@app.post("/data")
async def receive_data(data: DataRequest):
    print(f"Received from C++: {data}")
    return {"status": "OK", "echo": data.message}
