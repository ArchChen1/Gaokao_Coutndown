$(document).ready(function () {
    schedule(); // 先执行一次
});
function schedule() {
    // 创建对象来存储上午、下午、每日课程


    // 获取当前日期的星期几并显示相应的课程表
    var now = new Date();
    var week = now.getDay();

    // 映射星期几
    var dayMapping = ["Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"];
    var day = dayMapping[week];

    // 遍历特定日期的课程数组，并将每门课程显示在页面上
    if (courseSchedule[day]) {
        courseSchedule[day].morning.forEach(course => {
            AM += `${course}<br/>`;
        });
        courseSchedule[day].afternoon.forEach(course => {
            PM += `${course}<br/>`;
        });
    } else {
        AM = "";
        PM = "";
    }

    // 输出今天的课程表
    document.getElementById("morningschedule").innerHTML = AM;
    document.getElementById("afternoonschedule").innerHTML = PM;
}
//////////////////课程表个性化

$(document).ready(function () {//jquery ready命令，比window.onload好用，能重复使用
    set();//执行一次
});
function set() {
    document.getElementById("kechengbiaoBOX").style.position = "absolute";
    document.getElementById("kechengbiaoBOX").style.left = positionX + "%";
    document.getElementById("kechengbiaoBOX").style.top = positionY + "%";
    if (positionMODE == "1") {
        document.getElementById("kechengbiaoBOX").style.transform = "translate(-50%,-50%)";
    }
    document.getElementById("kechengbiaoBOX").style.backgroundColor = "rgba(0,0,0," + opacity + ")";
    document.getElementById("kechengbiaoBOX").style.padding = shadowSIZE + "rem";
    document.getElementById("kechengbiaoBOX").style.zIndex = order;
    ///////////////////////////////////////////////////////////////////////////////////
    document.getElementById("gap").style.fontSize = gapSIZE + "rem";
    if (hide == "0") {
        document.getElementById("kechengbiaoBOX").style.opacity = "0";
    }
    document.getElementById("morningschedule").style.color = colorAM;
    document.getElementById("afternoonschedule").style.color = colorPM;
}