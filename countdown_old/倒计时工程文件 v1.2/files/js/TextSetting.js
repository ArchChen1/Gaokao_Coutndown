$(document).ready(function () {//jquery ready命令，比window.onload好用，能重复使用
    applyStyles();//执行一次
});
//line1
var xianzaishiWENZI = ["华文行楷", "45rem", 200, "1rem 1rem 10rem #000001", "#ff99cc"];
//line2
var line2text = ["华文行楷", "30rem", 200, "1rem 1rem 6rem #000002", "#ff99cc"];
var Now_number = ["华文行楷", "50rem", 700, "1rem 1rem 10rem #000003", "#ffffff"];
var time = ["MS Gothic", "50rem", 700, "1rem 1rem 10rem #000004", "#ffffff"];
//line3
var line3text = ["华文行楷", "89rem", 200, "1rem 1rem 7rem #000005", "#ff99cc"];
var line3num = ["MV Boli", "100rem", 900, "1rem 1rem 8rem #000006", "#ffffff"];
var gaokao = ["华文行楷", "100rem", 200, "1rem 1rem 8rem #000007", "#ffffff"];
//line4
var Line4text = ["等线", "120rem", 1000, "1rem 1rem 10rem #000008", "#ff99cc"];
var Line4num = ["MV Boli", "225rem", 900, "1rem 1rem 10rem #000009", "#ffffff"];
var Line4text_lite = ["等线", "30rem", 1000, "1rem 1rem 10rem #000010", "#ff99cc"];
var Line4num_lite = ["MS Gothic", "50rem", 900, "1rem 1rem 10rem #000011", "#ffffff"];
//serlectorgroup
var selectors = ["xianzaishiWENZI", "line2text", "Now_number", "time", "line3text", "line3num", "gaokao", "Line4text", "Line4num", "Line4text_lite", "Line4num_lite"];

function ApplyStylesFormat(selector, styles) {
    $(selector).css({
        "font-family": styles[0],
        "font-size": styles[1],
        "font-weight": styles[2],
        "text-shadow": styles[3],
        "color": styles[4]
    });


}

function applyStyles() {
    selectors.forEach(selector => {
        // 查找相应的样式数组
        var stylesArray = window[selector];

        // 如果找到了样式数组，就应用样式
        if (stylesArray) {
            ApplyStylesFormat("." + selector, stylesArray);
        }
        else {
            console.log(`找不到名为 "${selector}" 的样式数组。`);
        }
    });
}