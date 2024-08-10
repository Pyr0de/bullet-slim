//Button index:
//0: Left
//1: Right
//2: Up
//3: Interact
//handleControl(buttonindex, buttonstate)
let handleControl = Module.cwrap("handleControl", null, ["int", "bool"])

document.getElementById('up').addEventListener("mousedown", () => {handleControl(2,1)})
document.getElementById('up').addEventListener("mouseup", () => {handleControl(2,0)})
