/**
 * Created by Guillaume on 02/05/2016.
 */



var cpuDatas = [0, 4, 0, 0, 3, 10, 45, 20, 3, 2, 2, 1, 4, 6, 2];
var ramDatas = [48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 60];
var diskDatas = [0, 0, 0, 0, 0, 0, 4, 50, 20, 10, 3, 0, 0, 0, 0];
var linkDatas = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];

var receiveDatas = [];
var chart;
var options = {
    title: 'Company Performance',
    curveType: 'function',
    legend: { position: 'bottom' }
};

var baseArray = [];
var cpt = 0;

google.charts.load('current', {'packages':['corechart']});
google.charts.setOnLoadCallback(drawChart);

var socket = io('http://localhost:3000');
socket.on('message', function (datas) {
    receiveDatas = datas;
    reload()
});



function drawChart() {
    var array = graphTab();
    var data = google.visualization.arrayToDataTable(array);

    chart = new google.visualization.LineChart(document.getElementById('curve_chart'));
    chart.draw(data, options);
}


function updateDataGraph(newCpuValue, newRamValue, newDiskValue, newLinkValue) {
    if (cpuDatas.length >= 15) {
        cpuDatas.splice(0, 1);
        cpuDatas.push(newCpuValue);

        ramDatas.splice(0, 1);
        ramDatas.push(newRamValue);

        diskDatas.splice(0, 1);
        diskDatas.push(newDiskValue);

        linkDatas.splice(0, 1);
        linkDatas.push(newLinkValue);
    }
    else {
        cpuDatas.push(newCpuValue);
        ramDatas.push(newRamValue);
        diskDatas.push(newDiskValue);
        linkDatas.push(newLinkValue);
    }
}

function graphTab() {
    var graphTab = [];
    graphTab.push(['i', 'CPU', 'RAM', 'Disk', 'Link']);
    for (var i = 0; i < cpuDatas.length; i++) {
        graphTab.push([cpt, cpuDatas[i], ramDatas[i], diskDatas[i], linkDatas[i]]);
        cpt++;
    }
    return graphTab;
}


function reload(){
    updateDataGraph(receiveDatas.data[0], receiveDatas.data[1], receiveDatas.data[2], receiveDatas.data[3]);

    var array = graphTab();
    var data = google.visualization.arrayToDataTable(array);

    chart = new google.visualization.LineChart(document.getElementById('curve_chart'));

    chart.draw(data, options);
}

var i = 0;
/*var interval = setInterval(function(){
    reload();
    i++;
    if (i == 15){
        clearInterval(interval)
    }

},1000);*/

