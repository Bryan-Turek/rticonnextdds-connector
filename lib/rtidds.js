/*jslint node: true, esnext: true */
'use strict';

const bindings = require('../build/Release/rtiddsconnector.node');
const EventEmitter = require('events').EventEmitter;

// console.log(bindings);

class Connector extends EventEmitter {
  constructor(configName, fileName) {
    super();
    this._pointer = bindings.connector_new(configName, fileName);
    this._data_available = false;

    var connector = this;
    var onDataAvailable = function(pointer){
      bindings.connector_wait_async(pointer, -1, function(err, res) {
        if (err) throw err;
        connector.emit('data');
        onDataAvailable(pointer);
      });
    };

    this.on('newListener', function(event, listener) {
      if( event === 'data' && connector._data_available === false) {
        connector._data_available = true;
        onDataAvailable(connector._pointer);
      }
    });
  }
  getReader(readerName) {
    return new Reader(this._pointer, readerName);
  }
  getWriter(writerName) {
    return new Writer(this._pointer, writerName);
  }
  delete(){
    return bindings.connector_delete(this._pointer);
  }
}

module.exports.Connector = Connector;

class Reader {
  constructor(pointer, readerName) {
    this._pointer = pointer;
    this._name = readerName;
    this._samples = new Samples(this._pointer, this._name);
  }
  getSamples() {
    return this._samples;
  }
  read() {
    bindings.reader_read(this._pointer, this._name);
  }
  take() {
    bindings.reader_take(this._pointer, this._name);
  }
  readAsync() {
    bindings.reader_read_async(this._pointer, this._name, function() {
      // console.log('successfully ran the readAsync function!');
    });
  }
  takeAsync() {
    bindings.reader_take_async(this._pointer, this._name, function() {
      // console.log('successfully ran the takeAsync function!');
    });
  }
  samplesLength() {
    return bindings.get_samples_length(this._pointer, this._name);
  }
  infosLength() {
    return bindings.get_infos_length(this._pointer, this._name);
  }
  isInfoValid(index) {
    return bindings.get_boolean_from_infos(this._pointer,this._name,index,'valid_data');
  }
  getNumber(index, name) {
    return bindings.get_number_from_samples(this._pointer, this._name, index, name);
  }
  getString(index, name) {
    return bindings.get_string_from_samples(this._pointer, this._name, index, name);
  }
  getBoolean(index, name) {
    return bindings.get_boolean_from_samples(this._pointer, this._name, index, name);
  }
  getJson(index) {
    return JSON.parse(bindings.get_json_sample(this._pointer, this._name, index));
  }
}

class Writer {
  constructor(pointer, writerName) {
    this._pointer = pointer;
    this._name = writerName;
  }
  write() {
    return bindings.writer_write(this._pointer, this._name);
  }
  writeAsync() {
    return bindings.writer_write_async(this._pointer, this._name, function() {
      // console.log('successfully ran the async function')
    });
  }
  setNumber(name, value) {
    bindings.set_number_into_samples(this._pointer, this._name, name, value);
  }
  setString(name, value) {
    bindings.set_string_into_samples(this._pointer, this._name, name, value);
  }
  setBoolean(name, value) {
    bindings.set_boolean_into_samples(this._pointer, this._name, name, value);
  }
  setJson(data) {
    for( let key in data ) {
      let value = data[key];
      if( typeof value === 'number' ) {
        this.setNumber(key, value);
      } else if( typeof value === 'string' ) {
        this.setString(key, value);
      } else if( typeof value === 'boolean' ) {
        this.setBoolean(key, value);
      } else if( typeof key === 'object' ) {
        console.log('Sorry, we do not handle objects at the moment');
      } else {
        console.log('No interface for:' + key);
      }
    }
  }
}

class Samples {
  constructor(pointer, name) {
    this._pointer = pointer;
    this._name = name;
  }
  length() {
    return bindings.get_samples_length(this._pointer, this._name);
  }
  getNumber(index, name) {
    return bindings.get_number_from_samples(this._pointer, this._name, index, name);
  }
  getString(index, name) {
    return bindings.get_string_from_samples(this._pointer, this._name, index, name);
  }
  getBoolean(index, name) {
    return bindings.get_boolean_from_samples(this._pointer, this._name, index, name);
  }
  getJson(index) {
    return JSON.parse(bindings.get_json_sample(this._pointer, this._name, index));
  }

}
