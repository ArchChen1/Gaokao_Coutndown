var courseSchedule = {
    Mon: {morning: ["周一", "语文", "数学", "生物", "化学", "英语"], afternoon: ["心理", "物理", "自习", "班会", " ", " "]},
    Tue: {morning: ["周二", "英语", "化学", "物理", "语文", "数学"], afternoon: ["体育", "生物", "自习", "艺体", " ", " "]},
    Wed: {morning: ["周三", "语文", "数学", "生物", "化学", "英语"], afternoon: ["心理", "语文", "语文", "艺体", " ", " "]},
    Thu: {morning: ["周四", "语文", "数学", "生物", "化学", "英语"], afternoon: ["心理", "语文", "语文", "艺体", " ", " "]},
    Fri: {morning: ["周五", "语文", "数学", "生物", "化学", "英语"], afternoon: ["心理", "语文", "语文", "艺体", " ", " "]},
    Sat: {morning: ["周六", "语文", "数学", "生物", "化学", "英语"], afternoon: ["心理", "语文", "语文", "回家！", " ", " "]},
    Sun: {morning: [" ", " ", " ", " ", " ", " "], afternoon: [" ", " ", " ", " ", " ", " "]},
};
var positionMODE = "2";
var positionX = "87";
var positionY = "0";
var shadowSIZE = "20";
var opacity = "0";
var gapSIZE = "30";
var order = "0";
var hide = "1";
var colorAM = "";
var colorPM = "";
var AM = "";
var PM = "";
////////////////////////////你可以更改内容，但请不要改变以上格式,如需更改以下内容请前往/bin/ScheduleOtherJSCode.js//////////////////////////
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
