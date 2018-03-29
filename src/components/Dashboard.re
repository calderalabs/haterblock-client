open Belt;

[%bs.raw {|require('./Dashboard.css')|}];

let component = ReasonReact.statelessComponent("Dashboard");

let make = _children => {
  ...component,
  render: _self =>
    <div className="Dashboard">
      <CommentList sentiment=Hateful />
      <CommentList sentiment=Negative />
      <CommentList sentiment=Neutral />
      <CommentList sentiment=Positive />
    </div>,
};
