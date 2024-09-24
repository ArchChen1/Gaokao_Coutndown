$(document).ready(function () {//jquery ready命令，比window.onload好用，能重复使用
    boxset();//先执行一次
    setInterval(function () { refresh() }, 7200000);//然后每2h刷新
});


//  X Y height
var line1 = ["17", "26", "110"];
selectors = ["line1"];
function ApplyStylesFormat(selector, styles) {
    $(selector).css({
        "left": styles[0] + "%",
        "top": styles[1] + "%",
        "line-height": styles[2] + "rem"
    });

}
function boxset() {
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