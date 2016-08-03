<!DOCTYPE html>
<html lang="zh-cn">
<head>
	<meta charset="utf-8">
	<meta http-equiv="X-UA-Compatible" content="IE=edge">
	<meta name="viewport" content="width=device-width, initial-scale=1">
	<title>玩家</title>

	<?php $this->load->view('css'); ?>
	<?php $this->load->view('js'); ?>

</head>

<body>

	<p>通过名字查询玩家</p>
	<form action="index.php?/player/name" method="post" enctype="multipart/form-data">
		role_name: <input type="text" name="role_name">
		<input type="submit" name="sub" value="submit">
	</form>

	<p>通过id查询玩家</p>
	<form action="index.php?/player/role" method="post" enctype="multipart/form-data">
		role_id: <input type="text" name="role_id">
		<input type="submit" name="sub" value="submit">
	</form>

	<?php $this->load->view('nav', TRUE); ?>
	<?php $this->load->view('help'); ?>

	<p>设置VIP</p>

	<div class="input-group">
		<span class="span-align-login input-group-addon " id="sizing-addon1">role_id:</span>
		<input type="text" class="form-control" id="role_id" placeholder="role_id" name="role_id">
	</div>
	<div class="input-group">
		<span class="span-align-login input-group-addon " id="sizing-addon1">lv:</span>
		<input type="text" class="form-control" id="lv" placeholder="lv" name="lv">
	</div>
	<div class="input-group">
		<span class="span-align-login input-group-addon " id="sizing-addon1">month:</span>
		<input type="text" class="form-control" id="month" placeholder="month" name="month">
	</div>

	<button id="btn-set-vip" class="btn btn-info" >
		<i class="glyphicon glyphicon-plus-sign"></i> set vip
	</button>

</body>

<script>

var set_vip_url = 'index.php?/player/set_vip?opt=0';

$(document).ready(function() {
	
	$("#help").click(function () {
		$('#helpModal').modal();
	});

	$("#btn-set-vip").click(function () {
		var id = document.getElementById('role_id').value,
		 lv = document.getElementById('lv').value,
		 month = document.getElementById('month').value;
		$.post(
			set_vip_url, 
			{
				'id': id,
				'lv': lv,
				'month': month,
			}, 
			function (rsp, status) {
				alert(rsp);
			});
	});

});

</script>

</html>
