open Belt;

[%bs.raw {|require('normalize.css/normalize.css')|}];

[%bs.raw {|require('font-awesome/css/font-awesome.css')|}];

[%bs.raw {|require('ladda/dist/ladda.min.css')|}];

[%bs.raw {|require('./components/Button.css')|}];

[%bs.raw {|require('./components/Link.css')|}];

[@bs.module "./registerServiceWorker"]
external registerServiceWorker : unit => unit = "default";

ReactDOMRe.renderToElementWithId(<App />, "root");

registerServiceWorker();
