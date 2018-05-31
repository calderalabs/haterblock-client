open Belt;

[%bs.raw {|require('./Landing.css')|}];

let component = ReasonReact.statelessComponent("Landing");

let make = (~onLogin: unit => unit, _children) => {
  ...component,
  render: (_) =>
    <div className="Landing">
      <div className="Landing__heading">
        <h1 className="Landing__title">
          (ReasonReact.stringToElement("Build a positive community"))
        </h1>
        <h2>
          (
            ReasonReact.stringToElement(
              "We use AI to help you identify and manage negative comments on your channel",
            )
          )
        </h2>
        <div className="Landing__callToAction">
          <button
            onClick=(_event => onLogin()) className="Button Button--primary">
            (ReasonReact.stringToElement("Connect With Youtube"))
          </button>
        </div>
      </div>
      <div className="Landing__features">
        <div className="Landing__feature">
          <div className="Landing__featureTitle">
            (ReasonReact.stringToElement("Analyze"))
          </div>

          <div className="Landing__featureDescription">
            (ReasonReact.stringToElement("Our AI will analyze your comments and automatically sort them by sentiment, separating comments that might need moderation from the rest."))
          </div>
        </div>

        <div className="Landing__feature">
          <div className="Landing__featureTitle">
            (ReasonReact.stringToElement("Take Action"))
          </div>

          <div className="Landing__featureDescription">
            (ReasonReact.stringToElement("We will notify you whenever you receive negative comments so that you can review and reject them from our dashboard."))
          </div>
        </div>

        <div className="Landing__feature">
          <div className="Landing__featureTitle">
            (ReasonReact.stringToElement("Reject Automatically"))
          </div>

          <div className="Landing__featureDescription">
            (ReasonReact.stringToElement("Optionally, you can activate automatic rejection of comments marked as hateful by our AI."))
          </div>
        </div>
      </div>
    </div>,
};
