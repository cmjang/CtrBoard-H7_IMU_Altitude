t <html><head><title>Network Settings</title>
t <script language=JavaScript>
t function changeConfirm(f){
t  if(!confirm('Are you sure you want to change\nthe Network settings?')) return;
t  f.submit();
t }
t </script></head>
i pg_header.inc
t <h2 align=center><br>Network Settings</h2>
t <p><font size="2">Here you can change the system <b>Network Settings</b>.
t  After you have changed the IP address, you need to change also the host IP address in 
t  you Internet browser to re-connect to target.<br><br>
t  This Form uses a <b>GET</b> method to send data to a Web server.</font></p>
t <form action=network.cgi method=get name=cgi>
t <input type=hidden value="net" name=pg>
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#aaccff>
t  <th width=40%>Item</th>
t  <th width=60%>Setting</th></tr>
# Here begin data setting which is formatted in HTTP_CGI.C module
t <tr><td><img src=pabb.gif>IP Address</td>
c a i <td><input type=text name=ip value="%s" size=18 maxlength=18></td></tr>
t <tr><td><img src=pabb.gif>Network Mask</td>
c a m <td><input type=text name=msk value="%s" size=18 maxlength=18></td></tr>
t <tr><td><img src=pabb.gif>Default Gateway</td>
c a g <td><input type=text name=gw value="%s" size=18 maxlength=18></td></tr>
t <tr><td><img src=pabb.gif>Primary DNS Server</td>
c a p <td><input type=text name=pdns value="%s" size=18 maxlength=18></td></tr>
t <tr><td><img src=pabb.gif>Secondary DNS Server</td>
c a s <td><input type=text name=sdns value="%s" size=18 maxlength=18></td></tr>
t </font></table>
# Here begin button definitions
t <p align=center>
t <input type=button name=set value="Change" onclick="changeConfirm(this.form)">
t <input type=reset value="Undo">
t </p></form>
i pg_footer.inc
. End of script must be closed with period.

