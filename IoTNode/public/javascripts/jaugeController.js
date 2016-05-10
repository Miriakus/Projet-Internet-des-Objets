/**
 * Created by Guillaume on 04/05/2016.
 */

var receiveData = [];
var socket = io('http://localhost:3000');
socket.on('message', function (data) {
    receiveData = data;
    reload();
});

var data;
var chart;

var options = {
    width: 1500, height: 500,
    redFrom: 90, redTo: 100,
    yellowFrom: 75, yellowTo: 90,
    minorTicks: 10,
    'majorTicks': ["0","25", "50","75", "100"]
};

google.charts.load('current', {'packages': ['gauge']});
google.charts.setOnLoadCallback(drawChart);

function drawChart() {

    data = google.visualization.arrayToDataTable([
        ['Label', 'Value'],
        ['CPU', 0],
        ['Memory', 0],
        ['Swap', 0],
        ['Disk', 0],
        ['Download', 0],
        ['Upload', 0]
    ]);

    chart = new google.visualization.Gauge(document.getElementById('chart_div'));

    chart.draw(data, options);
}

function reload() {
    data = google.visualization.arrayToDataTable([
        ['Label', 'Value'],
        ['CPU', receiveData.data.cpu.pcentUsed],
        ['RAM', receiveData.data.ram.pcentUsed],
        ['Swap', receiveData.data.swap.pcentUsed],
        ['Disk', receiveData.data.disk.pcentActive],
        ['Download', receiveData.data.network.debitDown / 1000000],
        ['Upload', receiveData.data.network.debitUp / 1000000]


    ]);
    chart = new google.visualization.Gauge(document.getElementById('chart_div'));
    chart.draw(data, options);
}