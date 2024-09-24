var switch1 = 1;
var switch2 = 1;
//Timer 1
var aimhours = 7;
var aimminutes = 0;
var aimseconds = 0;
//Timer 2
var aimhours2 = 8;
var aimminutes2 = 0;
var aimseconds2 = 0;


$(document).ready(function () {//jquery ready命令，比window.onload好用，能重复使用
    timer();//先执行一次
    setInterval(function () { timer() }, 1000);//然后每秒刷新
});
function timer() {
    var now = new Date();//【输出现在时间模块】
    var hours = now.getHours();//时
    var minutes = now.getMinutes();//分
    var seconds = now.getSeconds();//秒

    if (hours == aimhours && minutes == aimminutes && seconds == aimseconds && switch1 == 1) {
        location.reload();
        console.log("1")

    }
    if (hours == aimhours2 && minutes == aimminutes2 && seconds == aimseconds2 && switch2 == 1) {
        location.reload();
        console.log("1")

    }
}