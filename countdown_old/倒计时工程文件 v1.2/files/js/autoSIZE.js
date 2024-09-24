$(document).ready(function () {//jquery ready命令，比window.onload好用，能重复使用
    //先执行一次防闪屏
    autosize();
    //窗口加载好再执行，防止修改背景图片宽高无法获取元素
    window.onload = function () { autosize(); }
    //【网页窗口大小改变自动执行】
    window.addEventListener('resize', function () {
        autosize();
    }, false);
});


function autosize() {
    var ClientHeight = document.documentElement.clientHeight || document.body.clientHeight;//【自动调整字体大小模块】获取网页高
    var FontSize = (ClientHeight / 1080);//计算基准字体尺寸，用于rem单位，自动调整字体大小
    document.getElementsByTagName('html')[0].style['font-size'] = FontSize + 'px';//设置基准字体尺寸

    var ClientWidth = document.documentElement.clientWidth || document.body.clientWidth;//【自动调整背景大小模块】获取网页宽

    var img = new Image();//背景图片函数
    img.src = $('#backgroundIMG').attr('src');//从html获取src
    if ((ClientWidth / ClientHeight) >= (img.width / img.height)) {//比较窗口和背景图片的长宽比
        img.style.width = ClientWidth + "px";//窗口更宽就把图片宽设置成窗口宽
        img.style.height = "auto";//窗口更宽就把图片高设置成自动
    }
    else {
        img.style.width = "auto";//图片更宽就把图片宽设置成自动
        img.style.height = ClientHeight + "px";//图片更宽就把图片高设置成窗口高
    }

    var vdo = document.getElementById("backgroundVIDEO");//背景视频函数
    if ((ClientWidth / ClientHeight) >= (vdo.offsetWidth / vdo.offsetHeight)) {//比较窗口和背景视频的长宽比
        vdo.style.width = ClientWidth + "px";//窗口更宽就把视频宽设置成窗口宽
        vdo.style.height = "auto";//窗口更宽就把视频高设置成自动
    }
    else {
        vdo.style.width = "auto";//视频更宽就把视频宽设置成自动
        vdo.style.height = ClientHeight + "px";//视频更宽就把视频高设置成窗口高
    }

    var canvas = document.getElementById("canvas");
    if ((ClientWidth / ClientHeight) >= (canvas.offsetWidth / canvas.offsetHeight)) {//比较窗口和星星背景的长宽比
        canvas.style.width = ClientWidth + "px";//窗口更宽就把星星背景宽设置成窗口宽
        canvas.style.height = "auto";//窗口更宽就把星星背景高设置成自动
    }
    else {
        canvas.style.width = "auto";//星星背景更宽就把星星背景宽设置成自动
        canvas.style.height = ClientHeight + "px";//星星背景更宽就把星星背景高设置成窗口高
    }

    document.getElementById("weatherBOX").style.zoom = ClientHeight / 1080 * 125 + "%";//修改天气大小【该部分代码在textSETTINGS.js实现】

    document.getElementById("kechengbiaoBOX").style.padding = ClientHeight / 1080 * 20 + "px " + ClientHeight / 1080 * 20 + "px " + ClientHeight / 1080 * 20 + "px ";//修改课程表大小
}
