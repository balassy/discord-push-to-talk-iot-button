const express = require('express');
const morgan = require('morgan')
const robot = require("robotjs");

const app = express();
const port = 20314;
const apiKey = 'alma';

app.use(morgan('tiny'));

app.get('/health', onHealthRequest);
app.route('/discord/press').all(authorizeRequest).post(onDiscordPressRequest);
app.route('/discord/release').all(authorizeRequest).post(onDiscordReleaseRequest);

app.listen(port, onReady);

function onReady() {
  console.log(`Discord button listening on port ${port}!`);

  console.log('');
  console.log('Available commands:');
  console.log('   GET  /health');
  console.log('   POST /discord');
  console.log('');
  console.log('Request log:');
}

function onHealthRequest(req, res) {
  res.send('OK');
}

function onDiscordPressRequest(req, res) {
  console.log('Press');
  robot.keyToggle('0', 'down', ['control']);
  res.send('Pressed');
}

function onDiscordReleaseRequest(req, res) {
  console.log('Release');
  robot.keyToggle('0', 'up', ['control']);
  res.send('Released');
}

function authorizeRequest(req, res, next) {
  const apiKeyInRequest = req.headers['x-api-key'];
  
  if(!apiKeyInRequest) {
    return res.status(401).send('Unauthorized');
  }

  if(apiKeyInRequest !== apiKey) {
    return res.status(403).send('Access Denied');
  }

  next();
}