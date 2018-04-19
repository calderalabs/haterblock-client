open Belt;

[%bs.raw {|require('./CommentListFilters.css')|}];

let component = ReasonReact.statelessComponent("CommentListFilters");

type filters = {
  status: list(CommentData.Status.t),
  sentiment: list(CommentData.Sentiment.t),
};

let make = (~filters: filters, ~onFiltersChange: filters => unit, _children) => {
  let isStatusChecked = (status: CommentData.Status.t) =>
    filters.status |> List.has(_, status, (==));
  let onToggleStatus = (status: CommentData.Status.t) => {
    let newStatus =
      if (isStatusChecked(status)) {
        filters.status |> List.keep(_, s => s != status);
      } else {
        filters.status |> List.add(_, status);
      };
    onFiltersChange({...filters, status: newStatus});
  };
  let isSentimentChecked = (sentiment: CommentData.Sentiment.t) =>
    filters.sentiment |> List.has(_, sentiment, (==));
  let onToggleSentiment = (sentiment: CommentData.Sentiment.t) => {
    let newSentiment =
      if (isSentimentChecked(sentiment)) {
        filters.sentiment |> List.keep(_, s => s != sentiment);
      } else {
        filters.sentiment |> List.add(_, sentiment);
      };
    onFiltersChange({...filters, sentiment: newSentiment});
  };
  {
    ...component,
    render: _self =>
      <div className="CommentListFilters">
        <div className="CommentListFilters__filter">
          <input
            name="published"
            id="published"
            _type="checkbox"
            checked=(
              Js.Boolean.to_js_boolean(
                isStatusChecked(CommentData.Status.Published),
              )
            )
            onChange=(_event => onToggleStatus(CommentData.Status.Published))
          />
          <label className="CommentListFilters__label" htmlFor="published">
            (ReasonReact.stringToElement("Published"))
          </label>
        </div>
        <div className="CommentListFilters__filter">
          <input
            name="rejected"
            id="rejected"
            _type="checkbox"
            checked=(
              Js.Boolean.to_js_boolean(
                isStatusChecked(CommentData.Status.Rejected),
              )
            )
            onChange=(_event => onToggleStatus(CommentData.Status.Rejected))
          />
          <label className="CommentListFilters__label" htmlFor="rejected">
            (ReasonReact.stringToElement("Rejected"))
          </label>
        </div>
        <div className="CommentListFilters__filter">
          <input
            name="hateful"
            id="hateful"
            _type="checkbox"
            checked=(
              Js.Boolean.to_js_boolean(
                isSentimentChecked(CommentData.Sentiment.Hateful),
              )
            )
            onChange=(
              _event => onToggleSentiment(CommentData.Sentiment.Hateful)
            )
          />
          <label className="CommentListFilters__label" htmlFor="hateful">
            (ReasonReact.stringToElement("Hateful"))
          </label>
        </div>
        <div className="CommentListFilters__filter">
          <input
            name="negative"
            id="negative"
            _type="checkbox"
            checked=(
              Js.Boolean.to_js_boolean(
                isSentimentChecked(CommentData.Sentiment.Negative),
              )
            )
            onChange=(
              _event => onToggleSentiment(CommentData.Sentiment.Negative)
            )
          />
          <label className="CommentListFilters__label" htmlFor="negative">
            (ReasonReact.stringToElement("Negative"))
          </label>
        </div>
        <div className="CommentListFilters__filter">
          <input
            name="neutral"
            id="neutral"
            _type="checkbox"
            checked=(
              Js.Boolean.to_js_boolean(
                isSentimentChecked(CommentData.Sentiment.Neutral),
              )
            )
            onChange=(
              _event => onToggleSentiment(CommentData.Sentiment.Neutral)
            )
          />
          <label className="CommentListFilters__label" htmlFor="neutral">
            (ReasonReact.stringToElement("Neutral"))
          </label>
        </div>
        <div className="CommentListFilters__filter">
          <input
            name="positive"
            id="positive"
            _type="checkbox"
            checked=(
              Js.Boolean.to_js_boolean(
                isSentimentChecked(CommentData.Sentiment.Positive),
              )
            )
            onChange=(
              _event => onToggleSentiment(CommentData.Sentiment.Positive)
            )
          />
          <label className="CommentListFilters__label" htmlFor="positive">
            (ReasonReact.stringToElement("Positive"))
          </label>
        </div>
      </div>,
  };
};
