$(document).ready(function () {//jquery ready命令，比window.onload好用，能重复使用
    setInterval(function () { autoRELOAD() }, 1000);//然后每秒刷新
});

function autoRELOAD() {

    const iframe = document.getElementById('webreload');//刷新iframe，用iframe来传刷新时间
    window.open(iframe.src, 'webreload', '')//刷新iframe，用iframe来传刷新时间

    var updateTIME = "";
    window.addEventListener('message', function (e) {  // 监听 message 事件
        if (updateTIME == "") {
            updateTIME = e.data
        }
        else {
            if (updateTIME != e.data) {
                setInterval(function () { this.window.location.reload(); }, 1000);
            }
        }
    });
}