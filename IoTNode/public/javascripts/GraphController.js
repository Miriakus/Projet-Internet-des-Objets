/**
 * Created by Guillaume on 02/05/2016.
 */



var cpuData = [];
var ramData = [];
var swapData = [];
var diskData = [];
var downloadData = [];
var uploadData = [];

var receiveData = [];
var chart;
var options = {
    title: 'graph stat',
    curveType: 'function',
    legend: {position: 'bottom'}
};

var baseArray = [];
var cpt = 0;


/*var graph = {
    drawChart: function () {
        var array = graphTab();
        var data = google.visualization.arrayToDataTable(array);

        chart = new google.visualization.LineChart(document.getElementById('curve_chart'));
        chart.draw(data, options);
    },

    updateDataGraph: function(newCpuValue, newRamValue, newDiskValue, newLinkValue){
        cpuDatas.push(newCpuValue);
        ramDatas.push(newRamValue);
        diskDatas.push(newDiskValue);
        linkDatas.push(newLinkValue);
    },

    graphTab: function(){
        var graphTab = [];
        graphTab.push(['i', 'CPU', 'RAM', 'Disk', 'Link']);
        for (var i = 0; i < cpuDatas.length; i++) {
            graphTab.push([cpt, cpuDatas[i], ramDatas[i], diskDatas[i], linkDatas[i]]);
            cpt++;
        }
        return graphTab;
    },

    reload: function(){
        graph.updateDataGraph(receiveDatas.data[0], receiveDatas.data[1], receiveDatas.data[2], receiveDatas.data[3]);

        var array = graph.graphTab();
        var data = google.visualization.arrayToDataTable(array);

        chart = new google.visualization.LineChart(document.getElementById('curve_chart'));

        chart.draw(data, options);
    }


};
*/

function drawChartGraph() {
    var array = graphTab();
    var data = google.visualization.arrayToDataTable(array);

    chart = new google.visualization.LineChart(document.getElementById('curve_chart'));
    chart.draw(data, options);
}


/*function updateDataGraph(newCpuValue, newRamValue, newDiskValue, newLinkValue) {
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

}*/

function updateDataGraph(values){
    values.forEach(function(data){
        cpuData.push(data.cpu.pcentUsed);
        ramData.push(data.ram.pcentUsed);
        swapData.push(data.swap.pcentUsed);
        diskData.push(data.disk.pcentActive);
        downloadData.push(data.network.debitDown);
        uploadData.push(data.network.debitUp);

        graphTab();
    })
}

function graphTab() {
    var graphTab = [];
    graphTab.push(['i', 'CPU', 'RAM','SWAP','Disk', 'Download', 'Upload']);
    for (var i = 0; i < cpuData.length; i++) {
        graphTab.push([cpt, cpuData[i], ramData[i], swapData[i], diskData[i], downloadData[i], uploadData[i]]);
        cpt++;
    }
    return graphTab;
}

function drawGraph(values){
    console.log("OKKKK");
    updateDataGraph(values);
    google.charts.load('current', {'packages': ['corechart']});
    google.charts.setOnLoadCallback(drawChartGraph);
}

if(showGraph){
    console.log('ok');
    drawGraph(values)
}

/*function reload() {

    var array = graphTab();
    var data = google.visualization.arrayToDataTable(array);

    chart = new google.visualization.LineChart(document.getElementById('curve_chart'));

    chart.draw(data, options);
}*/

//module.exports = graph;


