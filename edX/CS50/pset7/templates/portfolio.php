<?php
include 'common.php';
?>
<table class="table table-striped" border=2 width="75%"cellpadding=4>
    <thead>
    <tr>
        <th colspan="1" BGCOLOR="#99CCFF">Symbol</th>
        <th colspan="1" BGCOLOR="#99CCFF">Name</th>
        <th colspan="1" BGCOLOR="#99CCFF">Shares</th>
        <th colspan="1" BGCOLOR="#99CCFF">Price</th>
        <th colspan="1" BGCOLOR="#99CCFF">Total</th>
    </tr>
    </thead>
    <tbody>
    <?php
    foreach ($positions as $position)
    {
        echo("<tr>");
        echo("<td>" . $position["symbol"] . "</td>");
        echo("<td>" . $position["name"] . "</td>");
        echo("<td>" . $position["shares"] . "</td>");
        echo("<td>$" . number_format($position["price"], 2) . "</td>");
        echo("<td>$" . number_format($position["total"], 2) . "</td>");
        echo("</tr>");
    }
    $total = 0;
    foreach ($positions as $position)
    {
        $total = $total + $position["price"];
        dump($total);
    }
    ?>
    </tr>
    <tr>
        <br/>
        <td colspan="4" BGCOLOR="#99CC00">Balance Cash</td>
        <td colspan="4" BGCOLOR="#99CC00">$<?=number_format($users[0]["cash"], 2)?></td>
    </tr>
    </tbody>
</table>

<script>
    $(“img”).hide();
</script>
