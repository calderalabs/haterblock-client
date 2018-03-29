open Belt;

[%bs.raw {|require('normalize.css/normalize.css')|}];

[%bs.raw {|require('./components/Button.css')|}];

[@bs.module "./registerServiceWorker"]
external registerServiceWorker : unit => unit = "default";

ReactDOMRe.renderToElementWithId(<App />, "root");

registerServiceWorker();
