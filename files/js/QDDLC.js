function QChr(name, x) {
    this.ctrl = document.getElementById(name);
    
    this.autoMove = 1; // 初始化自动移动为1
    this.isMoving = 0; // 初始化是否正在移动
    this.isDragging = 0;
    this.forward = 1; // 初始面朝为向左

    // 获取元素的变换矩阵
    const matrix = window.getComputedStyle(this.ctrl).transform;
    this.scale = 1; // 若无缩放，则默认缩放比例为 1

    if (matrix && matrix !== 'none') {
        // 将矩阵字符串转为数组
        const matrixValues = matrix.match(/matrix\((.+)\)/)[1].split(', ');
        // 获取缩放值 (matrix(a, b, c, d, e, f)) 中的 a 就是 scaleX，d 是 scaleY
        this.scale = parseFloat(matrixValues[0]);
    }

    // 计算缩放后的高度
    this.ctrl.style.transformOrigin = "top"; // 定义缩放基准点为top

    this.chrHeight = this.ctrl.offsetHeight * this.scale;
    this.defaultDis = document.getElementById('box').offsetHeight - this.chrHeight; //角色上端距离距离盒子高度
    console.log(name,this.chrHeight,this.scale,this.defaultDis)// ****debug****
    
    this.name = name;
    this.x = x;
    this.y = this.defaultDis;
    this.ctrl.style.left = this.x + 'px'; // 设置角色的初始x坐标
    this.ctrl.style.top = this.y + 'px'; // 设置角色的初始y坐标
    this.ctrl.onclick = this.onclick.bind(this);
    this.ctrl.onmouseover = this.onmouseover.bind(this); // 鼠标悬停事件
    this.ctrl.onmouseout = this.onmouseout.bind(this); // 鼠标离开事件
    this.bgurl1 = 'url(./files/images/' + name + '_sticker_1.png)';
    this.bgurl2 = 'url(./files/images/' + name + '_sticker_2.png)';
    // 预加载背景图片
    this.images = [];
    this.images[0] = new Image();
    this.images[0].src = './files/images/' + name + '_sticker_1.png';
    this.images[1] = new Image();
    this.images[1].src = './files/images/' + name + '_sticker_2.png';
    this.startAutoMove();
}
// 随机移动函数
QChr.prototype.randomMove = function () {
    if(this.autoMove == 0) return;
    if(this.isMoving == 1) return;
    this.isMoving = 1;
    let xSpeed = 1;
    let maxSpeed = 1;
    let ySpeed = 5;
    let acceleration = 0.5;
    let self = this;


    this.forward = Math.random() < 0.5 ? -1 : 1;
    if(this.forward == 1){
        this.ctrl.style.transform = `scale(${this.scale}) scaleX(-1)`; // 水平反转
    }else if(this.forward == -1){
        this.ctrl.style.transform = `scale(${this.scale}) scaleX(1)`;
    }
   
    function move() {
        
        // 增加速度
        xSpeed += acceleration;
        if (xSpeed > maxSpeed) xSpeed -= acceleration;

        ySpeed -= acceleration;

        // 移动角色
        
        self.x += xSpeed * self.forward;
        self.ctrl.style.left = self.x + 'px';

        // 角色轻微跳跃
        self.y -= ySpeed;
        self.ctrl.style.top = self.y + 'px';

        // 角色下降回到原始位置时，结束跳跃
        if (self.y >= self.defaultDis) {
            self.y = self.defaultDis; // 保证角色不会超出父容器底部
            self.ctrl.style.top = self.y + 'px';
            self.isMoving = 0; // 跳跃结束，重置标志位
            self.ctrl.style.backgroundImage=self.bgurl1;
            return; // 停止动画
            
        }

        // 当角色碰到屏幕边缘时，改变方向并反转角色
        if (self.x > window.innerWidth - self.ctrl.offsetWidth) {
            self.forward = -1; // 向左
        } else if (self.x < 0) {
            self.forward = 1; // 向右
        }
        // 继续下一帧
        requestAnimationFrame(move);
    }
    move();
    this.isMoving=0;
};

// 跳跃函数
QChr.prototype.jumping = function () {
    if(this.isMoving == 1) return; // 如果角色正在移动，直接返回，避免重复跳跃
    this.ctrl.style.backgroundImage=this.bgurl2;
    this.isMoving = 1; // 标志位，表示角色正在跳跃
    let ySpeed = 7; // 初始的Y轴速度
    let acceleration = 0.4; // 加速度
    let self = this;

    function move() {
        if (self.isDragging == 1) return;
        // 更新Y轴速度
        ySpeed -= acceleration;

        // 角色跳跃逻辑
        self.y -= ySpeed; 
        self.ctrl.style.top = self.y + 'px';

        // 角色下降回到原始位置时，结束跳跃
        if (self.y >= self.defaultDis) {
            self.y = self.defaultDis; // 保证角色不会超出父容器底部
            self.ctrl.style.top = self.y + 'px';
            self.isMoving = 0; // 跳跃结束，重置标志位
            self.ctrl.style.backgroundImage=self.bgurl1;
            return; // 停止动画
            
        }

        // 继续下一帧
        requestAnimationFrame(move);
    }

    move();
};
// 启动自动随机移动函数
QChr.prototype.startAutoMove = function () {
    let self = this;
    // 设置一个随机的延迟时间
    let delay = Math.random() * 1000; // 随机延迟0到1000毫秒
    setTimeout(function () {
        // 定时器开始后，角色开始自动移动
        self.autoMoveInterval = setInterval(function () {
            if (self.autoMove) {
                let isJump = Math.random() < 0.5 ? -1 : 1;// 二分之一概率发生移动
                if(isJump == 1)
                self.randomMove();
            }
        }, 1200); 
    }, delay);
};


QChr.prototype.onclick = function () {
    console.log('click');
};

QChr.prototype.onmouseover = function () {
    this.autoMove = 0; // 停止自动移动

    if (this.isMoving === 0 && this.isDragging != 1)
        this.jumping(); 
        this.jumpInterval = setInterval(() => {
            if (this.isMoving === 0 && this.isDragging != 1) {
                this.jumping(); // 如果动作结束，继续跳跃
            }
        }, 100); 
    // 添加鼠标按下事件，传入事件参数e
    let startX, startY, startMouseX, startMouseY;
    
    // 移除监听器
    this.ctrl.removeEventListener('mousedown', this.mouseDownHandler);
    this.ctrl.removeEventListener('touchstart', this.touchStartHandler);

    // 监听鼠标按下事件
    this.mouseDownHandler = (e) => {
        // 定义鼠标移动事件处理函数
        const mouseMove = (e) => {
            if (this.isDragging == 1) {
                const dx = e.clientX - startMouseX; // 水平方向移动距离
                const dy = e.clientY - startMouseY; // 垂直方向移动距离
                this.x = startX + dx;
                this.y = startY + dy;
                this.ctrl.style.left = startX + dx + 'px'; // 更新左边距
                this.ctrl.style.top = startY + dy + 'px';   // 更新上边距
            }
        };
    
        // 定义鼠标抬起函数
        const mouseUp = () => {
            this.isDragging = 0;
            this.isMoving = 0;
            this.autoMove = 1; 
            if (this.isMoving === 0 && this.isDragging != 1) {
                this.jumping();
            }
            // 移除事件监听器，防止多次触发
            document.removeEventListener('mousemove', mouseMove);
            document.removeEventListener('mouseup', mouseUp);
        };
    
        // 开始拖动
        this.isDragging = 1;
        this.ctrl.style.backgroundImage=this.bgurl2;
        startX = this.ctrl.offsetLeft; // 元素的初始左边距
        startY = this.ctrl.offsetTop;   // 元素的初始上边距
        startMouseX = e.clientX; // 鼠标的初始X坐标
        startMouseY = e.clientY; // 鼠标的初始Y坐标

        // 添加鼠标移动和抬起事件监听器
        document.addEventListener('mousemove', mouseMove);
        document.addEventListener('mouseup', mouseUp);
    };

     // 监听手机触摸事件
     this.touchStartHandler = (e) => { 
        const touchMove = (e) => {
            if (this.isDragging == 1) {
                const dx = e.touches[0].clientX - startMouseX; // 水平方向移动距离
                const dy = e.touches[0].clientY - startMouseY; // 垂直方向移动距离
                this.x = startX + dx;
                this.y = startY + dy;
                this.ctrl.style.left = startX + dx + 'px'; // 更新左边距
                this.ctrl.style.top = startY + dy + 'px';   // 更新上边距
            }
        };

        const touchEnd = () => {
            this.isDragging = 0;
            this.isMoving = 0;
            this.autoMove = 1; 
            if (this.isMoving === 0 && this.isDragging != 1) {
                this.jumping();
            }
            document.removeEventListener('touchmove', touchMove);
            document.removeEventListener('touchend', touchEnd);
        };

        // 开始拖动
        this.isDragging = 1;
        this.ctrl.style.backgroundImage=this.bgurl2;
        startX = this.ctrl.offsetLeft; // 元素的初始左边距
        startY = this.ctrl.offsetTop;   // 元素的初始上边距
        startMouseX = e.touches[0].clientX; // 触摸的初始X坐标
        startMouseY = e.touches[0].clientY; // 触摸的初始Y坐标

        document.addEventListener('touchmove', touchMove);
        document.addEventListener('touchend', touchEnd);
    };

    // 重新添加监听器
    this.ctrl.addEventListener('mousedown', this.mouseDownHandler);
    this.ctrl.addEventListener('touchstart', this.touchStartHandler);
};


QChr.prototype.onmouseout = function () {
    this.autoMove = 1; // 恢复自动移动
    clearInterval(this.jumpInterval); // 清除跳跃定时器
};


$(document).ready(function () {
    
// 初始化角色
let Sayori = new QChr('Sayori', 60);
let Natsuki = new QChr('Natsuki', 250);
let Yuri = new QChr('Yuri', 460);
let Monika = new QChr('Monika', 650);
let Anon = new QChr('Anon', 60);
let Yanami = new QChr('Yanami', 120);
let Rana = new QChr('Rana', 180);
let Tomori = new QChr('Tomori', 320);
let Soyo = new QChr('Soyo', 420);

});