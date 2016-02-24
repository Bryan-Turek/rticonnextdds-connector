/*jslint node: true, esnext: true */
'use strict';
const bindings = require('../build/Release/rtiddsconnector.node');
const EventEmitter = require('events').EventEmitter;
const libxmljs = require('libxmljs');
const fs = require('fs');

// asynchronous generator runner
function run(gen, iter) {
  (iter=gen((err, data) => (err && iter.throw(err)) || iter.next(data))).next();
}

class Connector extends EventEmitter {
  constructor(path, options) {
    super();

    if(typeof options !== 'undefined') {}

    this._participantLibraries = new Map();

    let self = this;

    // element function array
    let elementArray = [
      ['qos_library', function(child) {
        // create qos libraries
        // console.log('testing');
      }],
      ['types', function() {
        // create types
      }],
      ['domain_library', function() {
        // create domain libraries
      }],
      ['participant_library', function(child) {
        // create participant libraries
        let pName = child.attr('name').value();
        self._participantLibraries.set(pName, new ParticipantLibrary(pName, child));
      }]
    ];

    // function map for xml elements
    this.elementMap = new Map(elementArray);

    // run(function* (resume) {
    let contents = fs.readFileSync(path);
    let xmlDoc = libxmljs.parseXml(contents);


    let root = xmlDoc.get('//dds');

    for(let child of root.childNodes()) {
      if(child.type() === 'element') {
        self.elementMap.get(child.name())(child);
      }
    }

    // });

    // this._publishers = {};
  }
  participant(library, name) {
    return this._participantLibraries.get(library).participant(name);
  }
}

class ParticipantLibrary {
  constructor(name, child) {
    this._name = name;
    this._domainParticipants = new Map();

    let self = this;
    let participantArray = [
      ['domain_participant', function(cn) {
        self._domainParticipants.set(
          cn.attr('name').value(),
          new DomainParticipant(cn));
      }]
    ];
    this.participantMap = new Map(participantArray);

    for( let cn of child.childNodes() ) {
      if(cn.type() === 'element') {
        this.participantMap.get(cn.name())(cn);
      }
    }
  }
  get name() {
    return this._name;
  }
  participant(name) {
    return this._domainParticipants.get(name);
  }
}

class DomainParticipant {
  constructor(child) {
    this._name = child.attr('name').value();
    this._domainRef = child.attr('domain_ref').value();

    this._publishers = new Map();
    this._subscribers = new Map();

    let self = this;
    let domainArray = [
      ['publisher', function(name, cn) {
        self._publishers.set(name, new Publisher(name, cn));
      }],
      ['subscriber', function(name, cn) {
        self._subscribers.set(name, new Subscriber(cn));
      }]
    ];
    this.domainParticipantMap = new Map(domainArray);

    for( let cn of child.childNodes() ) {
      if(cn.type() === 'element') {
        let nodeName = cn.attr('name').value();
        this.domainParticipantMap.get(cn.name())(nodeName, cn);
      }
    }
  }
  get name() {
    return this._name;
  }
  get domainRef() {
    return this._domainRef;
  }
}

class Publisher {
  constructor(name, node) {
    this._name = name;
  }
  get name() {
    return this._name;
  }
}

class Subscriber {
  constructor(name, node) {
    this._name = name;
  }
  get name() {
    return this._name;
  }
}

// create new connector and set the participantLibrary
let connector = new Connector(__dirname +"/../../examples/nodejs/ShapeExample.xml");
let participant = connector.participant("MyParticipantLibrary", "Zero");
console.log(participant);
// let publisher = connector.publisher("MyPublisher");
// let reader = publisher.reader("MySquareReader");
// let writer = publisher.writer("MySquareWriter");
