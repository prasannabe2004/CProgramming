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
    $total = 0;
    foreach ($positions as $position)
    {
        $total = $total + ( $position["shares"] * $position["price"]);
        echo("<tr>");
        echo("<td>" . $position["symbol"] . "</td>");
        echo("<td>" . $position["name"] . "</td>");
        echo("<td>" . $position["shares"] . "</td>");
        echo("<td>$" . number_format($position["price"], 2) . "</td>");
        echo("<td>$" . number_format($position["total"], 2) . "</td>");
        echo("</tr>");
    }
    $total = number_format($total, 2);
    $balance = number_format($users[0]["cash"], 2);
    echo("<tr>");
    echo("<td colspan=4>" . "Grand Total" . "</td>");
    echo("<td colspan=1>" . $total . "</td>");
    echo("<td colspan=4>" . "Balance Cash" . "</td>");
    echo("<td colspan=1>" . $balance . "</td>");
    echo("</tr>");
    ?>
    </tbody>
</table>

<script>
    $(“img”).hide();
</script>
