
/**
 * Module dependencies
 */

var express = require('express'),
  routes = require('./routes'),
  api = require('./routes/api'),
  http = require('http'),
  path = require('path');

var app = module.exports = express();


/**
 * Configuration
 */

// All environments
app.set('port', process.env.PORT || 3000);
app.set('views', __dirname + '/views');
app.set('view engine', 'jade');
app.use(express.logger('dev'));
app.use(express.bodyParser());
app.use(express.methodOverride());
app.use(express.static(path.join(__dirname, 'public')));
app.use(app.router);

// Development only
if (app.get('env') === 'development') {
  app.use(express.errorHandler());
}

// Production only
if (app.get('env') === 'production') {
  // TODO
}


/**
 * Routes
 */

// Serve index and view partials
app.get('/', routes.index);
app.get('/partials/:name', routes.partials);

// API to query the database
app.get('/apks', api.getApkList);
app.get('/filter?name=":name"&version=":version"&developer=":developer"&genre=":genre"&userRating=":userRating"&releaseDate=":releaseDate"&fileSize=":fileSize"', api.filterApkList);

// Redirect all others to the index (HTML5 history)
app.get('*', routes.index);


/**
 * Start Server
 */

http.createServer(app).listen(app.get('port'), function () {
  console.log('Express server listening on port ' + app.get('port'));
});