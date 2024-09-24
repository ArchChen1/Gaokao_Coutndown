var starswitch = 1;
var color = "#ff99cc";
var starmax = 6;
var starmin = 4;

function generateRandomHex() {
	// 生成0到255之间的随机整数
	let randomInt = Math.floor(Math.random() * 256);

	// 将随机整数转换为十六进制字符串，并确保输出两位
	let hexString = randomInt.toString(16).toUpperCase().padStart(2, '0');
	return hexString;
}
function Star(id, x, y) {
	this.id = id;
	this.x = x;
	this.y = y;
	if (starswitch == 0) {
		starmax = -1;
		starmin = -1;
	}
	this.r = Math.floor(Math.random() * (starmax - starmin + 1)) + starmin;
	// 调用函数生成随机十六进制字符
	let randomHex = generateRandomHex();

	this.color = color + randomHex;

	this.direction = Math.random() < 0.5 ? -1 : 1; // 随机选择初始方向，-1 表示向左，1 表示向右
}


Star.prototype.draw = function () {
	ctx.fillStyle = this.color;
	ctx.shadowBlur = this.r * 2;
	ctx.beginPath();
	ctx.arc(this.x, this.y, this.r, 0, 2 * Math.PI, false);
	ctx.closePath();
	ctx.fill();
};

Star.prototype.move = function () {
	// 根据初始方向移动
	this.x += this.direction * 0.03; // 0.1是移动速度

	this.y -= 0.3;
	if (this.y <= -10) {
		this.y = HEIGHT + 10;
		this.x = Math.floor(Math.random() * WIDTH);
		this.direction = Math.random() < 0.5 ? -1 : 1; // 重新随机选择方向
	}

	this.draw();
};

Star.prototype.die = function () {
	stars[this.id] = null;
	delete stars[this.id];
}

var canvas = document.getElementById('canvas'),
	ctx = canvas.getContext('2d'),
	WIDTH,
	HEIGHT,
	stars = [],
	initStarsPopulation = 200;

setCanvasSize();
init();

function setCanvasSize() {
	WIDTH = document.documentElement.clientWidth,
		HEIGHT = document.documentElement.clientHeight;

	canvas.setAttribute("width", WIDTH);
	canvas.setAttribute("height", HEIGHT);
}

function init() {
	ctx.strokeStyle = "white";
	ctx.shadowColor = "white";
	for (var i = 0; i < initStarsPopulation; i++) {
		stars[i] = new Star(i, Math.floor(Math.random() * WIDTH), Math.floor(Math.random() * HEIGHT));
	}
	ctx.shadowBlur = 0;
	animate();
}

function animate() {
	ctx.clearRect(0, 0, WIDTH, HEIGHT);

	for (var i in stars) {
		stars[i].move();
	}

	requestAnimationFrame(animate);
}

function degToRad(deg) {
	return deg * (Math.PI / 180);
}
