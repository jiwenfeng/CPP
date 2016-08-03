<!DOCTYPE html>
<html lang="zh-cn">
<head>
  <meta charset="utf-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>活动</title>

  <?php $this->load->view('css'); ?>
  <?php $this->load->view('js'); ?>

</head>

<body>

  <?php $this->load->view('nav', TRUE); ?>


  <div class="row">

    <div class="col-md-3">
      <?php $this->load->view('server_list'); ?>
    </div>

    <div class="col-md-9">

      <div class="container">
        <ul class="nav nav-tabs">
          <li class="active"><a href="#home">列表</a></li>
          <li><a href="#menu1">批量处理(增加或者修改）</a></li>
        </ul>

        <div class="tab-content">
          <div id="home" class="tab-pane fade in active">
           <div class="panel panel-default">
            <div class="panel-heading" id="table-name">活动列表</div>
            <div class="panel-body"> 
              <div id="toolbar-promotions">
                <button id="btn-minus" class="btn btn-danger"> 
                  <i class="glyphicon glyphicon-minus"></i> 切换开启或者停止状态 
                </button>
              </div>
              <table id="promotions" class="table table-striped" data-toolbar="#toolbar-promotions" data-search="true" data-show-refresh="true" data-show-export="true" data-pagination="true" data-page-list="[10, 25, 50, 100, ALL]">
              </table>
            </div> 
          </div>

        </div>

        <div id="menu1" class="tab-pane fade">
          <div class="panel panel-default">
            <div class="panel-heading" id="table-name">批量处理</div>
            <div class="panel-body"> 
                <h3> 注意 </h3>
                <p>记得选择左边服务器</p>
                <div class="form-group">
                  <span class="span-align-login input-group-addon " id="sizing-addon1">id&nbsp;:</span>
                  <input type="text" class="form-control" id="modal-id" placeholder="id" name="id">
                </div>
                <div class="form-group">
                  <span class="span-align-login input-group-addon " id="sizing-addon1">活动名字:</span>
                  <input type="text" class="form-control" id="modal-name" placeholder="name" name="name">
                </div>
                <div class="form-group">
                  <span class="span-align-login input-group-addon " id="sizing-addon1">类型(1=开服天数,3=指定日期,4=按周开启):</span>
                  <input type="text" class="form-control" id="modal-type" placeholder="type" name="type">
                </div>
                <div class="form-group">
                  <span class="span-align-login input-group-addon " id="sizing-addon1">开始时间(类型1(天数) 类型3与4(具体时间09/29/2015 16:33)):</span>
                  <input type="text" class="form-control text-date" id="modal-start" placeholder="start_time" name="start_time">
                </div>
                <div class="form-group">
                  <span class="span-align-login input-group-addon " id="sizing-addon1">持续时间(秒):</span>
                  <input type="text" class="form-control" id="modal-duration" placeholder="duration_time" name="duration_time">
                </div>
                <div class="form-group">
                  <span class="span-align-login input-group-addon " id="sizing-addon1">间隔时间(秒):</span>
                  <input type="text" class="form-control" id="modal-loop" placeholder="loop_time" name="loop_time">
                </div>
              <center><button class="btn btn-default" id="btn-plus">提交</button></center>
            </div> 
          </div>
        </div>
      </div>

    </div>
  </div>

  <?php $this->load->view('help'); ?>

  <?php 
  $params = array(
    'id' => 'modal',
    'header' => 'add',
    'body' => 'body',
    'footer' => 'footer',
    );
  $this->load->view('modal', $params); 
  ?>

  <script>

  var set_url = 'index.php?/promotions/set?opt=1',
  del_url = 'index.php?/promotions/del?opt=1',
  list_url = 'index.php?/promotions/get_by_game?opt=1',
  list_col = [ 
  {field: 'check', checkbox: true},
  {field: 'id', title: 'ID', sortable: true },
  {field: 'name', title: '活动名',},
  {field: 'type', title: '类型', editable: { mod: 'inline' },},
  {field: 'start_time', title: '开启时间', editable: { mod: 'inline' },},
  {field: 'duration_time', title: '持续时间', editable: { mod: 'inline' },},
  {field: 'end_time', title: '结束时间'},
  {field: 'loop_time', title: '循环时间', editable: { mod: 'inline' },},
  {field: 'state', title: '状态 0:默认 1:进行中 2:已结束 3:待开启 4:暂停'},
  ],
  ip = '',
  port = 0;

  $('#btn-plus').click(function () {
    var servers = $('#server').bootstrapTable('getSelections'),
    type = document.getElementById('modal-type'         ).value,
    start_time = document.getElementById('modal-start').value,
    end_time  = 0;
    row = {
      'id'           : document.getElementById('modal-id'           ).value,
      'name'         : document.getElementById('modal-name'         ).value,
      'type'         : type,
      'start_time'   : start_time,//document.getElementById('modal-start'        ).value,
      'duration_time': document.getElementById('modal-duration'     ).value,
      'end_time'     : end_time,//document.getElementById('modal-end'          ).value,
      'loop_time'    : document.getElementById('modal-loop'         ).value,
      'state'        : 0,
    };

    if (type != 1) {
      row.start_time = Date.parse(start_time).getTime() / 1000;
    };

    //console.log(row);


    $.post(set_url, 
    {
      'servers': servers,
      'row': row,
    },
    function (rsp, status) {
      if (rsp.ret == 0) {
        $('#promotions').bootstrapTable('refresh');
        alert(rsp);
      }
    });
  });

  $("#btn-minus").click(function () {
    var b = confirm("确定要执行");
    if (!b) {
      return ;
    }

    var servers = $('#server').bootstrapTable('getSelections');
    if (servers.length == 0) {
      alert("请选择服务器");
      return ;
    };
    var rows = $('#promotions').bootstrapTable('getSelections');
    $.post(del_url, 
    {
      'servers': servers,
      'rows': rows,
    },
    function (rsp, status) {
      if (rsp.ret == 0) {
        $('#promotions').bootstrapTable('refresh');
      }
    });
  }); 

  function init_list() {
    $('#promotions').bootstrapTable({ url: list_url, columns: list_col });
  };

function to_date(ts) {
              var unixTimestamp = new Date(ts * 1000),
                  date = unixTimestamp.toLocaleString();
    return date;
    }

  function list_event() {
    $('#server').on('click-row.bs.table', function (e, row, element) {
      document.getElementById("table-name").innerHTML = row.name + "的活动列表";
      ip = row.ip;
      port = row.inner_port;
      var url = list_url + '&addr=' + row.ip + '&port=' + row.inner_port;
      $.getJSON(url, function(result) {
          for (index in result) {
              result[index].start_time = to_date(result[index].start_time);
              result[index].end_time = to_date(result[index].end_time);
              }
        $('#promotions').bootstrapTable('load', result);
      });
    });

    $('#promotions').on('editable-save.bs.table', function (e, field, row, value) {
      var servers = new Array();
      servers[0] = {'ip': ip, 'inner_port': port};
      $.post(set_url, 
      {
        'servers': servers,
        'row': row,
      }, 
      function (data, status) {
        alert(status);
      });
    });
  };

  $(document).ready(function() {
    init_list();
    init_server_list();
    list_event();
    $(".text-date").datetimepicker();

    $(".nav-tabs a").click(function(){
      $(this).tab('show');
    });
  });

  </script>

  </html>
