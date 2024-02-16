t <html>
t <head>
t <title>File upload</title>
t </head>
i pg_header.inc
t <h2 align=center><br>WEB Server File Upload</h2>
t <p><font size="3"><br>You can select files from your local PC 
t  and upload them to an SD Card connected to the evaluation board. 
t  Files will be stored to the <b>root folder</b> on SD Card.</font></p>
t <form action=upload.cgi method=POST enctype="multipart/form-data" name=fileupload>
t <p><font face="verdana"><br><br><br>Select a File to upload to SD Card:<br><br></font>
t  <input name="File" type="file" size="50" maxlength="1000">
t </p>
t <p align="center">
t  <input type=button value="Send" OnClick="this.form.submit()">
t </p>
i pg_footer.inc
. End of script must be closed with period.
