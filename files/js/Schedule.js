function Day(name){
    // 初始化每日早上/下午课程为空
    this.name = name;
    // 初始化每日课程数组，前者早上后者下午
    this.Class = [" ", " "];
}

$(document).ready(function () {
    schedule(); // 完成页面加载完执行
});

function schedule(){
    let nowadays = new Date();// 获得日期
    let week = nowadays.getDay() - 1;// 解出周几

    document.getElementById("morningSchedule").innerHTML = days[week].Class[0];
    document.getElementById("afternoonSchedule").innerHTML = days[week].Class[1];
    console.log(week);
    
}
// 初始化每日的对象
const days = ['Mon', 'Tue', 'Wed','Thu','Fri','Sat','Sun'].map(name => new Day(name));
days[0].Class = ["周一<br>语文","数学"]
days[1].Class = ["周二<br>语文","高等数学"]
days[2].Class = ["周一<br>语文","数学"]
days[3].Class = ["周二<br>语文","数学"]
days[4].Class = ["周一<br>语文","数学"]
days[5].Class = ["周二<br>语文","数学"]
days[6].Class = ["周一<br>语文","数学"]