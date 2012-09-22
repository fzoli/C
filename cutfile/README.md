<p>My first C program's name for Linux is <b>Cutfile</b>.</p>
<p>It needs two text file from command line parameter and it removes content of second file from the first file.</p>
<p><b>Usage:</b> ./cutfile [-a] [-f] [-v] first_file second_file</p>
<p>
  <b>Options:</b>
  <table>
    <tr>
      <td><i>-a</i></td>
      <td>Instead of removing, the content will be added to the end of the first file if the file is not contains the text already.</td>
    </tr>
    <tr>
      <td><i>-f</i></td>
      <td>Only the first occurrence will be removed from the file.<br>This option can not be used with option <i>-a</i>.</td>
    </tr>
    <tr>
      <td><i>-v</i></td>
      <td>Instead of write back the processed content to the first file, the content displayes at the standard output.</td>
    </tr>
  </table>
</p>
<p>
  <b>Return codes:</b>
  <table>
    <tr>
      <td><i>0</i></td>
      <td>When the content of first file has <b>changed</b>.<br>For example: content of second file has been removed or added.</td>
    </tr>
    <tr>
      <td><i>1</i></td>
      <td>Standard <b>error</b> code.<br>For example: file not exists, there is no more free memory.</td>
    </tr>
    <tr>
      <td><i>2</i></td>
      <td>When the content of first file has <b>NOT changed</b>.</td>
    </tr>
    <tr>
      <td><i>3</i></td>
      <td>Wrong parameters, <b>usage</b> appears.</td>
     </tr>
  </table>
</p>
<p>
  <b>Limitations:</b>
  <ul>
    <li>Both of file must have the same character encoding.</li>
    <li>There should be new line sign at the end of files.</li>
    <li>Windows' new line sign could case problems.</li>
    <li>BOM character is not allowed.</li>
  </ul>
</p>
