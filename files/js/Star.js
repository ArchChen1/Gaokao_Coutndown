var starswitch = 1; // 用于控制星星的开关变量，1表示开启
var color = "#ff99cc"; // 定义基础颜色
var starmax = 6; // 星星最大半径
var starmin = 4; // 星星最小半径

function generateRandomHex() {
	// 生成0到255之间的随机整数
	let randomInt = Math.floor(Math.random() * 256);

	// 将随机整数转换为十六进制字符串，并确保输出两位
	let hexString = randomInt.toString(16).toUpperCase().padStart(2, '0');
	return hexString;
}

function Star(id, x, y) {
	this.id = id; // 星星的唯一标识符
	this.x = x; // 星星的x坐标
	this.y = y; // 星星的y坐标
	// 调用函数生成随机十六进制字符，扩展颜色
	let randomHex = generateRandomHex();
	this.color = color + randomHex; // 给星星的颜色添加随机部分

	// 随机选择星星的移动方向，-1表示向左，1表示向右
	this.direction = Math.random() < 0.5 ? -1 : 1;
}

Star.prototype.draw = function () {
	// 设置星星的颜色
	ctx.fillStyle = this.color;

	// 设置星星的阴影模糊程度，模糊半径与星星大小相关
	ctx.shadowBlur = this.r * 2;

	// 开始绘制星星
	ctx.beginPath();
	ctx.arc(this.x, this.y, this.r, 0, 2 * Math.PI, false); // 以x, y为圆心，绘制半径为r的圆形星星
	ctx.closePath(); // 结束路径
	ctx.fill(); // 填充路径
};

Star.prototype.move = function () {
	// 根据初始方向移动x坐标
	this.x += this.direction * 0.03; // 星星横向移动速度，0.03表示缓慢移动

	// 星星向上移动y坐标
	this.y -= 0.3; // 0.3表示星星竖直方向上的移动速度

	// 如果星星超出了屏幕顶部，则重置星星的位置到屏幕底部
	if (this.y <= -10) {
		this.y = HEIGHT + 10; // 将y坐标重置到屏幕底部
		this.x = Math.floor(Math.random() * WIDTH); // 随机生成新的x坐标
		this.direction = Math.random() < 0.5 ? -1 : 1; // 随机选择新的移动方向
	}

	// 绘制星星
	this.draw();
};

Star.prototype.die = function () {
	// 移除星星实例
	stars[this.id] = null;
	delete stars[this.id];
}

var canvas = document.getElementById('canvas'), // 获取画布元素
	ctx = canvas.getContext('2d'), // 获取2D绘图上下文
	WIDTH, // 画布的宽度
	HEIGHT, // 画布的高度
	stars = [], // 存储所有星星的数组
	initStarsPopulation = 200; // 初始星星的数量

setCanvasSize(); // 设置画布尺寸
init(); // 初始化星星

function setCanvasSize() {
	// 获取屏幕宽高，设置为画布的宽高
	WIDTH = document.documentElement.clientWidth,
	HEIGHT = document.documentElement.clientHeight;

	// 设置画布的宽度和高度
	canvas.setAttribute("width", WIDTH);
	canvas.setAttribute("height", HEIGHT);
}

function init() {
	// 设置绘图样式
	ctx.strokeStyle = "white"; // 设置线条颜色为白色
	ctx.shadowColor = "white"; // 设置阴影颜色为白色

	// 初始化星星
	for (var i = 0; i < initStarsPopulation; i++) {
		stars[i] = new Star(i, Math.floor(Math.random() * WIDTH), Math.floor(Math.random() * HEIGHT)); // 创建星星对象并存储
	}

	ctx.shadowBlur = 0; // 移除阴影模糊效果
	animate(); // 开始动画
}

function animate() {
	// 清除画布
	ctx.clearRect(0, 0, WIDTH, HEIGHT); // 清除画布的内容

	// 更新并移动每颗星星
	for (var i in stars) {
		stars[i].move(); // 调用星星的move方法，更新星星位置
	}

	// 循环动画
	requestAnimationFrame(animate); // 使用requestAnimationFrame保持动画连续
}

function degToRad(deg) {
	// 将角度转换为弧度
	return deg * (Math.PI / 180); // 1度 = π/180 弧度
}
