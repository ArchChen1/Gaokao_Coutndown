$(document).ready(function () {
    countdown(); // 完成页面加载完执行
    setInterval(function () {
        countdown();
    }, 1000); 
});

function initCountdown(name,context){
    document.getElementById(name).innerHTML = context;
}

function formatNum(num) {
    return num.toString().padStart(2, '0'); // 保证数字为两位，前面补0
}

function countdown() {
    var nowDate = new Date(); // 获得当前日期和时间

    var year = nowDate.getFullYear();       
    var month = nowDate.getMonth() + 1;     // 从0开始，所以要加1
    var day = nowDate.getDate();            
    var hours = nowDate.getHours();         
    var minutes = formatNum(nowDate.getMinutes());     
    var seconds = formatNum(nowDate.getSeconds());    
    if(isCustom){
        var aimY = customY;
        var aimM = customM;
        var aimD = customD;
        var aimH = customH;
        var aimMin = customMin;
    }else{
        var aimY = year + 1;
        var aimM = 6;
        var aimD = 7;
        var aimH = 0;
        var aimMin = 0;
    }
    var aimDate = new Date(aimY, aimM - 1, aimD, aimH, aimMin, 0); // 目标日期
    var dTime = aimDate - nowDate; // 得到时间差ms
    var leftD = Math.floor(dTime / (1000 * 60 * 60 * 24)); // 剩余天数
    var leftH = Math.floor((dTime % (1000 * 60 * 60 * 24)) / (1000 * 60 * 60)); // 剩余小时
    var leftMin = Math.floor((dTime % (1000 * 60 * 60)) / (1000 * 60)); // 剩余分钟
    var leftS = Math.floor((dTime % (1000 * 60)) / 1000) + 1; // 剩余秒

    initCountdown("nowY",year);
    initCountdown("nowM",month);
    initCountdown("nowD",day);
    initCountdown("nowH",hours);
    initCountdown("nowMin",minutes);
    initCountdown("nowS",seconds);
    initCountdown("aimY",aimY);
    initCountdown("leftD",leftD);
    initCountdown("leftH", leftH);
    initCountdown("leftMin", leftMin);
    initCountdown("leftS", leftS);
}

var isCustom = false;
var customY = 2005;
var customM = 5;
var customD = 18;
var customH = 10;
var customMin = 10;