<!DOCTYPE html>
<html lang="zh-cn">
<head>
  <meta charset="utf-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>走马灯</title>

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
      <div class="input-group">
        <span class="span-align-login input-group-addon " id="sizing-addon1">内容:</span>
        <input type="text" class="form-control" id="contents" placeholder="搞起" name="contents">
      </div>
      <div class="input-group">
        <span class="span-align-login input-group-addon " id="sizing-addon1">开始时间:</span>
        <input type="text" class="text-date form-control" id="start" placeholder="0" name="start">
      </div>
      <div class="input-group">
        <span class="span-align-login input-group-addon " id="sizing-addon1">结束时间:</span>
        <input type="text" class="text-date form-control" id="end" placeholder="0" name="end">
      </div>
      <div class="input-group">
        <span class="span-align-login input-group-addon " id="sizing-addon1">间隔时间:</span>
        <input type="text" class="form-control" id="repeat" placeholder="0" name="repeat">
      </div>
      <button id="btn-set" class="btn btn-info"> <i class="glyphicon glyphicon-plus-sign"></i> Set </button>

      <div class="panel panel-default">
        <div class="panel-heading" id="table-name">走马灯列表</div>
        <div class="panel-body"> 
          <div id="toolbar-bulletin">
            <button id="btn-plus" class="btn btn-danger" disabled>
              <i class="glyphicon glyphicon-plus"></i> 增加
            </button>
            <button id="btn-pencil" class="btn btn-danger" disabled>
              <i class="glyphicon glyphicon-pencil"></i> 修改
            </button>
            <button id="btn-minus" class="btn btn-danger"> 
              <i class="glyphicon glyphicon-minus"></i> 删除 
            </button>
            <button id="btn-pause" class="btn btn-danger" > 
              <i class="glyphicon glyphicon-pause"></i> 暂停 
            </button>
          </div>
          <table 
          id="bulletin" 
          class="table table-striped"
          data-toolbar="#toolbar-bulletin"
          data-search="true"
          data-show-refresh="true"
          data-show-export="true"
          data-pagination="true"
          data-page-list="[10, 25, 50, 100, ALL]"
          >
        </table>
      </div> 
    </div>


  </div>
</div>

<?php $this->load->view('help'); ?>

<script>

var list_url = 'index.php?/bulletin/get?opt=1',
list_col = [
{field: 'check', checkbox: true},
{field: 'msg_id', title: 'ID', sortable: true },
{field: 'contents', title: '内容', editable: { type: 'textarea', mod: 'inline'},},
{field: 'start_time', title: '开启时间', editable: { mod: 'inline' },},
{field: 'end_time', title: '结束时间', editable: { mod: 'inline' },},
{field: 'repeat_time', title: '间隔时间', editable: { mod: 'inline' },},
{field: 'is_over', title: '状态 0:未播完 1:已播完'},
];
;

$("#btn-set").click(function () {
  var contents = document.getElementById("contents").value,
  start    = document.getElementById("start").value,
  end      = document.getElementById("end").value,
  repeat   = document.getElementById("repeat").value,
  server   = $('#server').bootstrapTable('getSelections');
  if (server.length < 1) {
    alert('没有选服务器');
    return ;
  };
  $.post(
    'index.php?/bulletin/revolving_req?opt=0',
    {
      'server': server, 
      'contents': contents,
      'start_time': start,
      'end_time': end,
      'repeat': repeat,
    },
    function (data, status) { 
      alert(JSON.stringify(data)); 
    });
});

$("#btn-minus").click(function () {
  var id     = $('#bulletin').bootstrapTable('getSelections');
  server = $('#server').bootstrapTable('getSelections');
  if (id.length < 1 || server.length < 1) {
    alert('选择服务器和走马灯消息');
    return ;
  };

  $.post(
    'index.php?/bulletin/minus?opt=0',
    {
      'id': id,
      'server': server, 
    },
    function (data, status) { 
      alert(JSON.stringify(data)); 
    });
});

$("#btn-pause").click(function () {
  var id     = $('#bulletin').bootstrapTable('getSelections');
  server = $('#server').bootstrapTable('getSelections');
  if (id.length < 1 || server.length < 1) {
    alert('选择服务器和走马灯消息');
    return ;
  };

  $.post(
    'index.php?/bulletin/pause?opt=0',
    {
      'id': id,
      'server': server, 
    },
    function (data, status) { 
      alert(JSON.stringify(data)); 
    });
});

function list_event() {
  $('#server').on('click-row.bs.table', function (e, row, element) {
    document.getElementById("table-name").innerHTML = "(" + row.name + ")的走马灯列表";
    ip = row.ip;
    port = row.inner_port;
    var url = list_url + '&addr=' + row.ip + '&port=' + row.inner_port;
    $.getJSON(url, function(result) {
      $('#bulletin').bootstrapTable('load', result);
    });
  });
}

function init_list() {
  init_server_list();
  $('#bulletin').bootstrapTable({ url: list_url, columns: list_col });
}

$(document).ready(function() {
  init_list();
  list_event();

  $(".text-date").datetimepicker();
});

</script>

</html>
