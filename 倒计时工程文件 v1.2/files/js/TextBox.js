var BOXswitch = 1;
var APIswitch = 1;
var line1 = "悟已往之不谏，知来者之可追。";
var line2 = "——『归去来兮辞』 陶渊明";

$(document).ready(function () {//jquery ready命令，比window.onload好用，能重复使用
    textbox();//先执行一次
    setInterval(function () { refresh() }, 7200000);//然后每2h刷新
});
function textbox() {
    $("#textline1").text(line1);
    $("#textline2").text(line2);

    if (BOXswitch == 0) {
        $(".textBOX").hide();

    }
    if (APIswitch == 1 && BOXswitch == 1) {
        $.ajax({
            dateType: "json",
            type: "get",
            url: "https://v1.hitokoto.cn/",
            success: function (data) {
                var f = "——『" + data.from + "』";
                line1 = data.hitokoto;

                if (data.from_who != data.from && data.from_who == null) {//作者未知，来源已知的情况
                    line2 = f;
                } else if (data.from_who == data.from && data.from != null) {//作者和来源相同的情况
                    line2 = "—— " + data.from_who;
                } else if (data.from_who == data.from && data.from == null) {//作者和来源都未知
                    line2 = "—— 佚名";
                } else {//作者和来源不同
                    line2 = f + data.from_who;
                }

                $("#textline1").text(line1);
                $("#textline2").text(line2);
            }
        })
    }

}