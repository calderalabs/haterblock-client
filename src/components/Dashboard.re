open Belt;

[%bs.raw {|require('./Dashboard.css')|}];

let component = ReasonReact.statelessComponent("Dashboard");

let make = (~user: UserData.User.t, _children) => {
  let name = user.name;
  {
    ...component,
    render: _self =>
      <div className="Dashboard">
        <div> (ReasonReact.stringToElement({j|Hello $(name)|j})) </div>
        <div>
          <CommentList sentiment=Hateful />
          <CommentList sentiment=Negative />
          <CommentList sentiment=Neutral />
          <CommentList sentiment=Positive />
        </div>
      </div>,
  };
};
