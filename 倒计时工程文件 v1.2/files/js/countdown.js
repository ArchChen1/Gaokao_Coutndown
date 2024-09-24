var setting = 1;//高考时间1 指定日期0
var settingDAY = "2023/06/06 06:06:06";
$(document).ready(function () {//jquery ready命令，比window.onload好用，能重复使用
    refresh();//先执行一次
    setInterval(function () { refresh() }, 1000);//然后每秒刷新

});
function refresh() {
    var theDAY;
    var now = new Date();//【输出现在时间模块】
    var nowYear = now.getFullYear();//年
    var nowMonth = now.getMonth() + 1;//月
    var nowDay = now.getDate();//日
    var nowHour = now.getHours();//时
    var nowMinute = now.getMinutes();//分
    var nowSecond = now.getSeconds();//秒
    if (setting == 1) {
        if (nowMonth < 6) {
            theDAY = nowYear + "/06/07 9:00:00";
        }
        else if (nowMonth == 6 && nowDay < 7) {
            theDAY = nowYear + "/06/07 9:00:00";
        }
        else {
            theDAY = nowYear + 1 + "/06/07 9:00:00";
        }
    }
    else {
        theDAY = settingDAY;
    }

    function check(i) {//【修正：小于10在数字前面加0】
        if (0 <= i && i < 10) { i = "0" + i }
        else if (-10 < i && i < 0) { i = "0" + -i }
        else if (i <= -10) { i = -i }
        return i;
    }

    nowMonth = check(nowMonth)//加0修正
    nowDay = check(nowDay)
    nowHour = check(nowHour)
    nowMinute = check(nowMinute)
    nowSecond = check(nowSecond)
    document.getElementsByClassName("nowYear")[0].innerHTML = nowYear;//【输出】
    document.getElementsByClassName("nowMonth")[0].innerHTML = nowMonth;
    document.getElementsByClassName("nowDay")[0].innerHTML = nowDay;
    document.getElementsByClassName("nowHour")[0].innerHTML = nowHour;
    document.getElementsByClassName("nowMinute")[0].innerHTML = nowMinute;
    document.getElementsByClassName("nowSecond")[0].innerHTML = nowSecond;
    /////////////////////////////////////////
    //【倒数模块】
    var endtime = new Date(theDAY);
    var lefttime = parseInt((endtime.getTime() - now.getTime()) / 1000);//毫秒除以1000得到秒

    var daojishiNIAN = endtime.getFullYear();//得到倒数的年
    var leftDay = parseInt(lefttime / (24 * 60 * 60))//通过秒计算剩余日时分秒
    var leftHour = parseInt(lefttime / (60 * 60) % 24);
    var leftMinute = parseInt(lefttime / 60 % 60);
    var leftSecond = parseInt(lefttime % 60);
    leftDay = check(leftDay)//加0修正
    leftHour = check(leftHour)
    leftMinute = check(leftMinute)//加0修正
    leftSecond = check(leftSecond)
    document.getElementsByClassName("daojishiNIAN")[0].innerHTML = daojishiNIAN;//【输出】
    document.getElementsByClassName("leftDay")[0].innerHTML = leftDay;
    document.getElementsByClassName("leftHour")[0].innerHTML = leftHour;
    document.getElementsByClassName("leftMinute")[0].innerHTML = leftMinute;
    document.getElementsByClassName("leftSecond")[0].innerHTML = leftSecond;
}