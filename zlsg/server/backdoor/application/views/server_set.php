<!DOCTYPE html>
<html lang="zh-cn">
<head>
  <meta charset="utf-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>游戏设置</title>

  <?php $this->load->view('css'); ?>
  <?php $this->load->view('js'); ?>

</head>

<body>

    <?php $this->load->view('nav', TRUE); ?>
    <?php $this->load->view('help'); ?>

    <div class="row">

        <div class="col-md-3">
          <?php $this->load->view('server_list'); ?>
      </div>

      <div class="col-md-9">
        <div class="panel panel-default">
            <div class="panel-heading">GM命令开关</div>
            <div class="panel-body"> 

                <p id="gm-state-cur">点击服务器的名字可获取当前服务器GM状态</p>

                <div class="input-group">
                    <span class="span-align-login input-group-addon " id="sizing-addon1">1开0关:</span>
                    <input type="text" class="form-control" id="gm-state" placeholder="1" name="gm-state">
                </div>

                <button id="gm-set" class="btn btn-danger">
                    <i class="glyphicon glyphicon-cog"></i> set
                </button>

            </div>
        </div>
    </div>

</div>




</body>

<script>

var gm_url = 'index.php?/server_set/gm?opt=0';

$(document).ready(function() {
    init_server_list();

    $('#server').on('click-row.bs.table', function (e, row, element) {
        var a = Array();
        a[0] = row;
      $.post(gm_url, 
      {
        'servers': a,
        'set': 2,
    }, 
    function (rsp, status) {
        var state = "关闭";
        if (rsp.data == 1) {
            state = "打开";
        }

        document.getElementById('gm-state-cur').innerHTML = row.name + "当前GM状态: " + state;
    });
  });

    $("#help").click(function () {
        $('#helpModal').modal();
        $('#helpModal').on('show.bs.modal', function() {
        });
    });

    $("#gm-set").click(function () {
        var set = document.getElementById("gm-state").value,
        servers = $('#server').bootstrapTable('getSelections'),
        b = false;

        if (servers.length < 1) {
            alert("请先勾选服务器");
            return ;
        };

        var b = confirm("确定执行操作");
        if (!b) { 
            return ;
        }

        $.post(gm_url, 
        {
            'servers' : servers,
            'set'    : set
        },
        function (rsp, status) {
            alert(rsp.data);
        });
    });
});

</script>

</html>
