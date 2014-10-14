<?php
include 'common.php';
?>
<table class="table table-striped">
    <thead>
        <tr>
            <th>Symbol</th>
            <th>Name</th>
            <th>Shares</th>
            <th>Price</th>
            <th>Total</th>
        </tr>
    </thead>
    <tbody>
    <?php
	    foreach ($portfolio as $position)
        {   
            echo("<tr>");
            echo("<td>" . $position["symbol"] . "</td>");
            echo("<td>" . $position["name"] . "</td>");
            echo("<td>" . $position["shares"] . "</td>");
            echo("<td>$" . number_format($position["price"], 2) . "</td>");
            echo("<td>$" . number_format($position["total"], 2) . "</td>");
            echo("</tr>");
        }
    ?>
    <tr>
        <br/>
        <td colspan="4">CASH</td>
        <td>$<?=number_format($users[0]["cash"], 2)?></td>
    </tr>
    </tbody>
</table>

<script>
    $(“img”).hide();
</script>
