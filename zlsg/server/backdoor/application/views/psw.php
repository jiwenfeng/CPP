 <!DOCTYPE html>
 <html lang="zh-cn">
 <head>
  <meta charset="utf-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>后台</title>

  <?php $this->load->view('css'); ?>
  <?php $this->load->view('js'); ?>

</head>

<body>
    <?php $this->load->view('nav'); ?>

    <style type="text/css">
    .span-align-login { 
        width: 120px;
    }
    </style>

    <div class="container">
        <div class="row">
            <div class="col-md-4 col-md-offset-4">

                <div class="panel panel-default">
                    <div class="panel-heading">修改密码:</div>
                    <div class="panel-body">

                        <div class="input-group">
                            <span class="span-align-login input-group-addon " id="sizing-addon1">老密码:</span>
                            <input type="password" class="form-control" id="oldpsw" placeholder="oldpassword" name="oldpassword">
                        </div>

                        <div class="input-group">
                            <span class="span-align-login input-group-addon " id="sizing-addon1">新密码:</span>
                            <input type="password" class="form-control" id="psw" placeholder="password" name="password">
                        </div>

                        <div class="input-group">
                            <span class="span-align-login input-group-addon " id="sizing-addon1">重复新密码:</span>
                            <input type="password" class="form-control" id="repsw" placeholder="repassword" name="repassword">
                        </div>

                    </div>
                </div>

                <center>

                    <button type="button" class="btn btn-info" id="signin">
                        <i class="glyphicon glyphicon-user"></i>
                        修改
                    </button>

                    <button type="button" class="btn btn-info" id="signout">
                        <i class="glyphicon glyphicon-plane"></i>
                        退出
                    </button>

                </center>

            </div>
        </div>
    </div>

    <?php $this->load->view('help'); ?>

</body>

<script>

function check_psw() {
    var psw = document.getElementById("psw"    ).value;
    if (psw == "" || psw.length < 6) {
        return false;
    }

    return psw == document.getElementById("repsw"    ).value;
}

$(document).ready(function() {
    $("#help").click(function() {
        $('#helpModal').modal();
        $('#helpModal').on('show.bs.modal', function() {
        });
    });

    $("#signin").click(function () {
        if (!check_psw()) {
            alert("密码错误 不能少于6位或者两次密码不一样");    
            return ;
        }

        var oldpsw       = document.getElementById("oldpsw"   ).value,
        psw        = document.getElementById("psw"    ).value,
        //group      = document.getElementById("group"  ).value == 'admin' ? 0 : 1,
        url        = "index.php?/login/set_psw";

        $.post(
            url,
            {
                'oldpsw'   : oldpsw,
                'psw'    : psw,
                //'group'  : group,
            },
            function (data, status) {
                var info = '';

                if (data.ret != 0) {
                    info = data.info;
                } else {
                    info = "成功";
                    //location.reload(true);
                    //window.location.href = 'index.php?/login';
                }

                alert(info + " " + JSON.stringify(data));   
            });
    });

 $("#signout").click(function () {
        var //name       = document.getElementById("name"   ).value,
            //psw        = document.getElementById("psw"    ).value, 
            url        = "index.php?/login/signout";

            $.post(
                url,
                {
               // 'name'   : name,
                //'psw'    : psw,
            },
            function (data, status) {
                var info = '';
                if (data.ret != 0) {
                    info = data.info;
                } else {
                    info = "成功";
                }

                alert(info + " " + JSON.stringify(data));
                window.location.href = 'index.php?/login';
            });
        });
});

 </script>
 </html>
