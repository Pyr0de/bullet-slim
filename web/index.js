//Button index:
//0: Left
//1: Right
//2: Up
//3: Interact
//handleControl(buttonindex, buttonstate)
let handleControl = Module.cwrap("handleControl", null, ["int", "bool"])

window.oncontextmenu = function(event) {
     event.preventDefault();
     event.stopPropagation();
     return false;
};


document.getElementById('up').addEventListener("touchstart", () => {handleControl(2,1)})
document.getElementById('up').addEventListener("touchend", () => {handleControl(2,0)})

document.getElementById('left').addEventListener("touchstart", () => {handleControl(0,1)})
document.getElementById('left').addEventListener("touchend", () => {handleControl(0,0)})

document.getElementById('right').addEventListener("touchstart", () => {handleControl(1,1)})
document.getElementById('right').addEventListener("touchend", () => {handleControl(1,0)})

document.getElementById('interact').addEventListener("touchstart", () => {handleControl(3,1)})
document.getElementById('interact').addEventListener("touchend", () => {handleControl(3,0)})

document.getElementById('pause').addEventListener("touchstart", () => {handleControl(4,1)})
document.getElementById('pause').addEventListener("touchend", () => {handleControl(4,0)})

document.getElementById('fullscreen').addEventListener("touchstart", () => {
	if (window.innerWidth != screen.width || window.innerHeight != screen.height) {
		document.documentElement.requestFullscreen();
	}
})

document.addEventListener("touchstart", () => {
	document.getElementById("buttons").className = "shown"
})
document.addEventListener("keydown", () => {
	document.getElementById("buttons").className = "hidden"
})
