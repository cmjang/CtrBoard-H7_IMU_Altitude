t <html><head><title>Format SD Card</title>
t <script language=JavaScript>
t  function formatConfirm(f){
t   if(!confirm('Are you sure you want to format SD Card?')) return;
t   f.submit();
t  }
t </script></head>
i pg_header.inc
t <h2 align=center><br>Format SD Card</h2>
t <p><font size="3"><br>Before the first usage or when the FAT file system
t  is corrupted, you need to format SD memory card.<br><br>
t  It is strongly advised to format SD Memory Card before the first usage 
t  with this utility. It will optimize the cluster size and cluster alignment 
t  for the best SD Card performance.</font></p>
t <form action=index.htm method=post name=form1>
t <p><font size="3"><br>SD Card Volume Label:
c f 1 <input type=text name=label size=11 maxlength=11 value="%s">
t </font></p>
t <input type=hidden value="yes" name=format>
t <p align="center">
t  <input type=button value="Format SD Card" onclick="formatConfirm(this.form)">
t </p></form>
i pg_footer.inc
. End of script must be closed with period.
