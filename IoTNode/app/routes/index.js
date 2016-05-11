var express = require('express');
var router = express.Router();

var dataBaseRequest = require('../Controllers/DataBaseRequests');


/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('index', { title: 'Express' });
});

router.get('/dataRequest', function(req, res, next){
  res.render('dataRequest',{values: [], showGraph: false});
});

router.post('/dataRequest', dataBaseRequest.search);

module.exports = router;
