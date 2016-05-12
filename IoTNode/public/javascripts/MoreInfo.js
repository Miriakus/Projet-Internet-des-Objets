/**
 * Created by Guillaume on 09/05/2016.
 */


function showCPUInfo() {
    $('#cpu').html(`
            user: ${receiveData.data.cpu.user} tick <br>
            nice: ${receiveData.data.cpu.nice} tick <br>
            system: ${receiveData.data.cpu.system} tick <br>
            idle: ${receiveData.data.cpu.idle} tick <br>
            pcentUsed: ${receiveData.data.cpu.pcentUsed} % `);
}

function showRamInfo() {
    $('#ram').html(`
            total: ${receiveData.data.ram.total} ko<br>
            free: ${receiveData.data.ram.free} ko<br>
            cached: ${receiveData.data.ram.cached} ko<br>
            used: ${receiveData.data.ram.used} ko<br>
            pcentUsed: ${receiveData.data.ram.pcentUsed} % `);
}

function showSwapInfo(){
    $('#swap').html(`
        total: ${receiveData.data.swap.total} ko <br>
        free: ${receiveData.data.swap.free} ko <br>
        cached: ${receiveData.data.swap.cached} ko <br>
        used: ${receiveData.data.swap.used} ko <br>
        pcentUsed: ${receiveData.data.swap.pcentUsed} % `);
}

function showDiskInfo(){
    $('#disk').html(`
        totalRead: ${receiveData.data.disk.totalRead} ko <br>
        totalWrite: ${receiveData.data.disk.totalWrite} ko <br>
        totalTimeActive: ${receiveData.data.disk.totalTimeActive} ms <br>
        debitRead: ${receiveData.data.disk.debitRead} ko/s <br>
        debitWrite: ${receiveData.data.disk.debitWrite} ko/s <br>
        pcentActive: ${receiveData.data.disk.pcentActive} % `);
}

function showDownloadInfo(){
    $('#download').html(`
        totalDown: ${receiveData.data.network.totalDown} o <br>
        debitDown: ${receiveData.data.network.debitDown} o/s `);
}

function showUploadInfo(){
    $('#upload').html(`
        totalUp: ${receiveData.data.network.totalUp} o <br>
        debitUp: ${receiveData.data.network.debitUp} o/s `);
}

$('thead td button').on('click', function () {
    var id = $(this).attr('data-value');
    moreInfo();
    $('#' + id).toggleClass('hide');
});

function moreInfo() {

    if($('td#cpu').is(':visible')){
        showCPUInfo()
    }
    if($('td#ram').is(':visible')){
        showRamInfo()
    }
    if($('td#swap').is(':visible')){
        showSwapInfo()
    }
    if($('td#disk').is(':visible')){
        showDiskInfo()
    }
    if($('td#download').is(':visible')){
        showDownloadInfo()
    }
    if($('td#upload').is(':visible')){
        showUploadInfo()
    }

    setInterval(function(){
        if($('td#cpu').is(':visible')){
            showCPUInfo()
        }
        if($('td#ram').is(':visible')){
            showRamInfo()
        }
        if($('td#swap').is(':visible')){
            showSwapInfo()
        }
        if($('td#disk').is(':visible')){
            showDiskInfo()
        }
        if($('td#download').is(':visible')){
            showDownloadInfo()
        }
        if($('td#upload').is(':visible')){
            showUploadInfo()
        }
    },1000)
}





